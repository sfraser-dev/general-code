/*
 * YUV format converter.
 *
 * Supports standard YUV formats and special hardware formats.
 * Further supports either binary or text mode I/O making it
 * useful with testbenches.
 *
 * TODO: 
 * - Better chroma handling - 422->420 for example chooses funny pixels.
 * - Chroma interpolation
 * - Interlace
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdarg.h>
#include <string.h>
#include "stdsizes.c"

void help() {
    fprintf(stderr, "YUV format converter: yconvert [options] [in_file] [out_file]\n");
    fprintf(stderr, "-h, --help                        : Show this help\n");
    fprintf(stderr, "-H, --format-help                 : Show valid format strings\n");
    fprintf(stderr, "-s, --size <format>               : Size of YUV frames ie 640x480, vga, cif\n");
    fprintf(stderr, "-f, --format-in <format>          : Input YUV format\n");
    fprintf(stderr, "-F, --format-out <format>         : Output YUV format\n");
    fprintf(stderr, "-t, --text-in[=<fmt-string>]      : Use text mode for input stream\n");
    fprintf(stderr, "-T, --text-out[=<fmt-string>]     : Use text mode for output stream\n");
    fprintf(stderr, "-b, --bits-per-pixel-in <format>  : Input bits per pixel (default: 8)\n");
    fprintf(stderr, "-B, --bits-per-pixel-out <format> : Output bits per pixel (default: bpp-in)\n");
    fprintf(stderr, "-v, --verbose                     : Show status\n\n");
    fprintf(stderr, "    --mb-map <int>                : MB format chroma map\n\n");
    fprintf(stderr, "In text mode an appropriate conversion may be specified with:\n");
    fprintf(stderr, "  -T\"\%%x\\n\" or --text-in=\"%%i\"\n\n");  // I'd have thought '-T "%x\n"' would be OK, but it's not.  The exact format here is needed. 
    fprintf(stderr, "If no input/output file is specifed stdin/stdout are used.\n");
    
    exit(0);
}

enum {
    ERROR = -1,
    YUV_420, 
    YUV_422, 
    YUV_444, 
    YUY2,   
    UYVY,
    UYVYi,
    MB_420, 
    MB_420i, 
    MB_422, 
    MB_422i,
    Y44_420,
    Y44_422,
    Y88_420,
    Y84P_420,
};

// formats we recognise
struct _yuv_format {
    char *code;
    char *desc;
    int mode;
} yuv_formats[] = {
    // various ways of saying planar 420
    { "yuv420", "Planar 4:2:0",                 YUV_420 },
    { "yuv",    "Planar 4:2:0",                 YUV_420 },
    { "iyuv",   "Planar 4:2:0",                 YUV_420 },
    { "420",    "Planar 4:2:0",                 YUV_420 },
    { "i420",   "Planar 4:2:0",                 YUV_420 },
    // other planar                
    { "yuv422", "Planar 4:2:2",                 YUV_422 },
    { "yuv444", "Planar 4:4:4",                 YUV_444 },
    // interleaved
    { "yuy2",   "Interleaved 4:2:2",            YUY2 },
    { "uyvy",   "Interleaved 4:2:2",            UYVY },
    { "uyvyi",  "Interleaved 4:2:2 (fields)",   UYVYi },
    // special
    { "mb",     "Macroblock 4:2:0",             MB_420 },
    { "mb420",  "Macroblock 4:2:0",             MB_420 },
    { "mbi",    "Macroblock 4:2:0 (fields)",    MB_420i },
    { "mb420i", "Macroblock 4:2:0 (fields)",    MB_420i },
    { "mb422",	"Macroblock 4:2:2",	            MB_422 },
    { "mb422i",	"Macroblock 4:2:2 (fields)",    MB_422i },
    { "y44",    "Tiled packed 4x4 4:2:0",       Y44_420 },
    { "y88",    "Tiled planar 8x8 4:2:0",       Y88_420 },
    { "y84p",   "Tiled planar 8x4 4:2:0",       Y84P_420 },
    // last
    { NULL, NULL }
};

typedef struct _Yuv {
    int width, height;
    struct _yuv_format *format;
    int size;
    int cx_div, cy_div;
    int bits_per_pel, bytes_per_pel;
    unsigned int *data;
} Yuv;

// global option data
struct _yuv_format *format_in = yuv_formats, *format_out = yuv_formats; // defualt yuv420
int text_in, text_out;
char *text_in_format="%i", *text_out_format="%i\n";
char *text_in_format_orig="%i", *text_out_format_orig="%i\\n";
Yuv in, out;
int bpp_in=0, bpp_out=0;
int mbmap = 0;
int verbose;
FILE *fin, *fout;

#define bytes_per_pel(bits_per_pel) ((bits_per_pel+7)/8)

// print error and exit
void error(char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "yconvert: ");
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    exit(-1);
}

// The heart of the matter.
// Map (x,y) coordinates to luma/chroma pixel indexs within the data array.
// Called for both input + output data.
int idx(Yuv *yuv, int plane, int x, int y) {
    static int map_0[8] = {0,1,2,3,4,5,6,7};
    static int map_1[8] = {4,5,0,1,6,7,2,3};

    switch (yuv->format->mode) {
    case YUV_420:
    case YUV_422: 
    case YUV_444:     
    {
        int size = yuv->width * yuv->height;
        int csize = size / (yuv->cx_div*yuv->cy_div);
        switch (plane) {
        case 0: return x + (y*yuv->width);
        case 1: return size + (x/yuv->cx_div) + ((y/yuv->cy_div)*(yuv->width/yuv->cx_div));
        case 2: return size + csize + (x/yuv->cx_div) + ((y/yuv->cy_div)*(yuv->width/yuv->cx_div));
        }
        error("Invalid plane\n");
        break;
    }
    case YUY2: 
        switch (plane) {
        case 0: return (x*2) + 0 + (y*2*yuv->width);
        case 1: return ((x/2)*4) + 1 + (y*2*yuv->width);
        case 2: return ((x/2)*4) + 3 + (y*2*yuv->width);
        }
        error("Invalid plane\n");
        break;
    case UYVY:   
    {
        switch (plane) {
        case 0: return (x*2) + 1 + (y*2*yuv->width);
        case 1: return ((x/2)*4) + 0 + (y*2*yuv->width);
        case 2: return ((x/2)*4) + 2 + (y*2*yuv->width);
        }
        error("Invalid plane\n");
        break;
    }
    case UYVYi:
    {
        int i;
        yuv->format->mode = UYVY;
        i = idx(yuv, plane, x, (y/2) + ((y&1) * (yuv->height/2)));
        yuv->format->mode = UYVYi;
        return i;
        break;   
    }
    case MB_420: 
    {
        int *map = mbmap ? map_1 : map_0;
        switch (plane) {
        case 0: return ((x/16)+(y/16)*(yuv->width/16))*512+(x%16)+(y%16)*16;
        case 1: return (((x/2)/8)+((y/2)/8)*(yuv->width/16))*512+map[(x/2)%8]+((y/2)%8)*16+256;
        case 2: return (((x/2)/8)+((y/2)/8)*(yuv->width/16))*512+map[(x/2)%8]+((y/2)%8)*16+264;
        }
        error("Invalid plane\n");
        break;
    }
    case MB_420i:
    {
        int *map = mbmap ? map_1 : map_0;
        switch (plane) {
        case 0: return (y%2)*((yuv->height/16+1)/2)*(yuv->width/16)*512+((x/16)+((y/2)/16)*(yuv->width/16))*512+(x%16)+((y/2)%16)*16;
        case 1: return ((y/2)%2)*((yuv->height/16+1)/2)*(yuv->width/16)*512+(((x/2)/8)+(((y/2)/2)/8)*(yuv->width/16))*512+map[(x/2)%8]+(((y/2)/2)%8)*16+256;
        case 2: return ((y/2)%2)*((yuv->height/16+1)/2)*(yuv->width/16)*512+(((x/2)/8)+(((y/2)/2)/8)*(yuv->width/16))*512+map[(x/2)%8]+(((y/2)/2)%8)*16+264;
        }
        error("Invalid plane\n");
        break;
    }
    case MB_422:
    {
        int *map = mbmap ? map_1 : map_0;
        switch (plane) {
        case 0: return ((x/16)+(y/16)*(yuv->width/16))*512+(x%16)+(y%16)*16;
        case 1: return (((x/2)/8)+(y/16)*(yuv->width/16))*512+map[(x/2)%8]+(y%16)*16+256;
        case 2: return (((x/2)/8)+(y/16)*(yuv->width/16))*512+map[(x/2)%8]+(y%16)*16+264;
        }
        error("Invalid plane\n");
        break;
    }
    case MB_422i:
    {
        int i;
        yuv->format->mode = MB_422;
        i = idx(yuv, plane, x, (y/2) + ((y&1) * (yuv->height/2)));
        yuv->format->mode = MB_422i;
        return i;
        break;
    }
    case Y44_420:
        switch (plane) {
        case 0: return (((x / 4) + (y / 4) * (yuv->width / 4)) * 32) + (x % 4) + ((y % 4) * 4);
        case 1: return (((x/2) / 2) + ((y/2) / 2) * (yuv->width / 4)) * 32 + ((x/2) % 2) + ((y/2) % 2) * 4 + 16;
        case 2: return (((x/2) / 2) + ((y/2) / 2) * (yuv->width / 4)) * 32 + ((x/2) % 2) + ((y/2) % 2) * 4 + 16 + 8;
        }
        error("Invalid plane\n");
        break;
/*    case Y88_420:
         {
            int xm8 = x % 8;    // location within tile
            int ym8 = y % 8;
            int xd8 = x / 8;    // location of tile
            int yd8 = y / 8;
            int size = yuv->width * yuv->height;
            int csize = size / (yuv->cx_div*yuv->cy_div);
            switch (plane) {
            case 0: return (xd8 * 64) + (yd8 * yuv->width * 8) + xm8 + (ym8 * 8);
            case 1: return size;   // not implemented (for now)
            case 2: return size + csize;
            }
            error("Invalid plane\n");
            break;
         }
*/
    /* planar tiled */
    case Y88_420:
    case Y84P_420:
         {
            int tile_x = 0, tile_y = 0;
            int xm, ym;
            int xd, yd;
            int _x = x, _y = y;
            int offset = 0;
            int size = yuv->width * yuv->height;
            int csize = size / (yuv->cx_div*yuv->cy_div);
            int tile_size;
            int width = yuv->width;

            switch (yuv->format->mode) {
            case Y88_420:
                tile_x = 8;
                tile_y = 8;
                break;
            case Y84P_420:
                tile_x = 8;
                tile_y = 4;
                break;
            }
            tile_size = tile_x * tile_y;

            if (plane != 0) {
                width /= yuv->cx_div;
                _x /= yuv->cx_div;
                _y /= yuv->cy_div;
                offset = size;
            }
            if (plane == 2) offset += csize;
         
            xm = _x % tile_x;    // location within tile
            ym = _y % tile_y;
            xd = _x / tile_x;    // location of tile
            yd = _y / tile_y;

            return offset + (xd * tile_size) + (yd * width * tile_y) + xm + (ym * tile_x);
         }

    case Y44_422:
        error("Format not implemented\n");
        break;
    }
    error("Invalid format\n");
    return 0;
}

// create YUV frame data
void alloc_yuv(Yuv *yuv, struct _yuv_format *format, int width, int height, int bpp) {
    yuv->width = width;
    yuv->height = height;
    yuv->format = format;
    yuv->bits_per_pel = bpp;
    yuv->bytes_per_pel = (bpp+7)/8;
    switch (format->mode) {
    case YUV_420:
        yuv->cx_div = 2;
        yuv->cy_div = 2;
        yuv->size = width * height * 3 / 2;
        break;
    case YUV_422: 
        yuv->cx_div = 2;
        yuv->cy_div = 1;
        yuv->size = width * height * 2;
        break;
    case YUV_444: 
        yuv->cx_div = 1;
        yuv->cy_div = 1;
        yuv->size = width * height * 3;
        break;
    case YUY2: 
        yuv->cx_div = 2;
        yuv->cy_div = 1;
        yuv->size = width * height * 2;
        break;
    case UYVY:
    case UYVYi:
        yuv->cx_div = 2;
        yuv->cy_div = 1;
        yuv->size = width * height * 2;
        break;
    case MB_420: 
    case MB_420i: 
        yuv->cx_div = 2;
        yuv->cy_div = 2;
        yuv->size = width * height * 2;
        break;
    case MB_422: 
    case MB_422i: 
        yuv->cx_div = 2;
        yuv->cy_div = 1;
        yuv->size = width * height * 2;
        break;
    case Y44_420:
        yuv->cx_div = 2;
        yuv->cy_div = 2;
        yuv->size = width * height * 2;
        break;
    case Y44_422:
        yuv->cx_div = 2;
        yuv->cy_div = 1;
        yuv->size = width * height * 2;
        break;
    case Y88_420:
        yuv->cx_div = 2;
        yuv->cy_div = 2;
        yuv->size = width * height * 3 / 2;
        break;
    case Y84P_420:
        yuv->cx_div = 2;
        yuv->cy_div = 2;
        yuv->size = width * height * 3 / 2;
        break;
    default:
        error("Unknown format %i\n", format->mode);
        break;
    }
    yuv->data = malloc(yuv->size * sizeof(int));
    memset(yuv->data, 0, yuv->size * sizeof(int));
    if (NULL == yuv->data)
        error("Failed to allocate frame with dimensions %ix%i and format %s\n", width, height, format->desc);
}

void show_sizes() {
    struct stdsizes_t *s = stdsizes;
    fprintf(stdout, "\n************************************************\n");
    fprintf(stdout, "Codes which can be used with -s:\n\n");
    while (s->width) {
        fprintf(stdout, "%-10s %4ix%-4i [%s]\n", s->code, s->width, s->height, s->desc);
        s++;
    }
}

// Convert string to appropriate image size
int parse_size(char *s, int *width, int *height) {
    struct stdsizes_t *ss = stdsizes;
    if (s == NULL) return 0;
    if (*s == 0) return 0;

    // common names
    while (ss->width) {
        if (strcmp(ss->code, s) == 0) {
            *width = ss->width;
            *height = ss->height;
            return 1;
        }
        ss++;
    }
    
    // ie 512x384
    *width = strtol(s, &s, 0);
    if (*s == 0) return 0;
    s++;
    if (*s == 0) return 0;
    *height = strtol(s, &s, 0);
    
    if (*width <= 0) return 0;
    if (*height <= 0) return 0;

    return 1;
}

void show_formats() {
    int pos=0;
    fprintf(stdout, "\n************************************************\n");
    fprintf(stdout, "Codes which can be used with -f and -F:\n\n");
    while (yuv_formats[pos].code != NULL) {
        fprintf(stdout, "%-10s           [%s]\n", yuv_formats[pos].code, yuv_formats[pos].desc);
        pos++;
    }
}

// find input/output format
struct _yuv_format *get_format(char *format) {
    int pos=0;
    while (yuv_formats[pos].code != NULL) {
#ifdef WIN32		
        if (!_stricmp(yuv_formats[pos].code, format)) return yuv_formats + pos;
#else
        if (!strcasecmp(yuv_formats[pos].code, format)) return yuv_formats + pos;
#endif  
        pos++;
    }
    return NULL;
}

// process the format string provided at the command line for special characters
char *escapes(char *s) {
    int len = strlen(s);
    char *t, *r;
    
    if (len == 0) return NULL;
    t = malloc(len+1);
    r = t;
    while (*s) {
        if (*s == '\\') {
            s++;
            switch (*s) {
            case 'n': *t++ = '\n'; s++; break;
            case 'r': *t++ = '\r'; s++; break;
            case 't': *t++ = '\t'; s++; break;
            case '\\': *t++ = '\\'; s++; break;
            default: *t++ = '\\';
            }
        } else *t++ = *s++;
    }
    return r;
}

int read_yuv() {
    int i;
    if (text_in) {
        for (i=0; i<in.size; i++) {
            int tmp;
            if (1 != fscanf(fin, text_in_format, &tmp)) return 0;
            in.data[i] = tmp;
        }
    } else {
        for (i=0; i<in.size; i++)
            if (1 != fread(in.data + i, in.bytes_per_pel, 1, fin))
                return 0;
    }
    return 1;
}

int write_yuv() {
    int i;
    if (text_out) {
        int i;
        for (i=0; i<out.size; i++) {
            int tmp = out.data[i];
            if (fprintf(fout, text_out_format, tmp) < 0) return 0;
        }
    } else {
        for (i=0; i<out.size; i++)
            if (1 != fwrite(out.data + i, out.bytes_per_pel, 1, fout))
                return 0;
    }
    return 1;
}

void copy_pel(int p, int x, int y) {
    unsigned int idx_i = idx(&in, p, x, y);
    unsigned int tmp = in.data[idx_i];
    unsigned int idx_o;
    // shift pel
    if (out.bits_per_pel <= in.bits_per_pel)
        tmp = tmp >> (in.bits_per_pel - out.bits_per_pel);
    else
        tmp = tmp << (out.bits_per_pel - in.bits_per_pel);
    // mask pel
    tmp = tmp & ((1 << out.bits_per_pel)-1);
    idx_o = idx(&out, p, x, y);
    out.data[idx_o] = tmp;
}

int main(int argc, char *argv[]) {
    int x, y;
    int args_left=0;
    int frames=0;
    
    // Command line
    while (1) {
        #define MBMAP 256
        static struct option long_options[] = {
            { "help",               no_argument,        NULL, 'h' },
            { "format-help",        no_argument,        NULL, 'H' },
            { "verbose",            no_argument,        NULL, 'v' },
            { "size",               required_argument,  NULL, 's' },
            { "format-in",          required_argument,  NULL, 'f' },
            { "format-out",         required_argument,  NULL, 'F' },
            { "text-in",            optional_argument,  NULL, 't' },
            { "text-out",           optional_argument,  NULL, 'T' },
            { "bits-per_pixel-in",  required_argument,  NULL, 'b' },
            { "bits-per-pixel-out", required_argument,  NULL, 'B' },
            { "mb-map",             required_argument,  NULL, MBMAP },
            { 0, 0, 0, 0 }
        };
        int long_options_index = -1;
        int c = getopt_long(argc, argv, "hHs:f:F:t::T::b:B:v", long_options, &long_options_index);
        if (c == -1) break;
        
        switch (c) {
        case 'h':
            help();
            exit(0);
            break;
        case 'H':
            show_sizes();
            show_formats();
            exit(0);
            break;
        case 's':
            if (0 == parse_size(optarg, &in.width, &in.height)) error("Couldn't get input size\n");
            break;
        case 'f':
            format_in = get_format(optarg);
            break;
        case 'F':
            format_out = get_format(optarg);
            break;
        case 't':
            text_in = 1;
            text_in_format = optarg ? escapes(optarg) : text_in_format;  // optional
            text_in_format_orig = optarg ? optarg : text_in_format;  // optional
            break;
        case 'T':
            text_out = 1;
            text_out_format = optarg ? escapes(optarg) : text_out_format;  // optional
            text_out_format_orig = optarg ? optarg : text_out_format;  // optional
            break;
        case 'b':
            bpp_in = strtol(optarg, NULL, 0);
            break;
        case 'B':
            bpp_out = strtol(optarg, NULL, 0);
            break;
        case 'v':
            verbose = 1;
            break;
        case MBMAP:
            mbmap = atoi(optarg);
            break;
        case '?':
        default:
            error("Unknown option.\n");
            return 0;
        }
        
    }
    
    // check frame size makes sense
    if (in.width <= 0 || in.height <= 0) error("Invalid size specified %ix%i\n", in.width, in.height);
    
    if (format_in == NULL) error("Invalid input format\n");
    if (format_out == NULL) error("Invalid output format\n");
    
    // stitch up the inputs and outputs
    args_left = argc-optind;
    switch (args_left) {
    case 0:
        fin = stdin;
        fout = stdout;
        break;
    case 1:
        fin = fopen(argv[optind], text_in ? "r" : "rb");
        fout = stdout;
        break;
    case 2:
        fin = fopen(argv[optind], text_in ? "r" : "rb");
        fout = fopen(argv[optind+1], text_out ? "w" : "wb");
        break;
    default:
        error("Too many arguments provided\n");
        break;
    }
    
    if (fin == NULL) error("Failed to open input file \"%s\"\n",   fin ==stdin ?"stdin" :argv[optind]);
    if (fout == NULL) error("Failed to open output file \"%s\"\n", fout==stdout?"stdout":argv[optind+1]);
 
    if (bpp_in == 0) bpp_in = 8;
    if (bpp_out == 0) bpp_out = bpp_in;   

    // allocate input and outputs frames
    alloc_yuv(&in, format_in, in.width, in.height, bpp_in);
    alloc_yuv(&out, format_out, in.width, in.height, bpp_out);
    
    // show how we're configured
    if (verbose) {
        fprintf(stderr, "input : name=\"%s\" format=%s cdiv=%ix%i size=%i text=%i(%s)\n", fin ==stdin ?"stdin" :argv[optind],   in.format->code,  in.cx_div,  in.cy_div,  in.size,  text_in, text_in_format_orig);
        fprintf(stderr, "output: name=\"%s\" format=%s cdiv=%ix%i size=%i text=%i(%s)\n", fout==stdout?"stdout":argv[optind+1], out.format->code, out.cx_div, out.cy_div, out.size, text_out, text_out_format_orig);
    }
    
    // loop over the input frame, converting and writing to the output frame
    while (read_yuv()) {
    
        // copy luma
        for (y=0; y<in.height; y++) 
            for (x=0; x<in.width; x++) 
                copy_pel(0, x, y);

        // copy chroma
        // Increment according to the chroma scaling factors of the output - 
        // this changes the 'old' behaviour where if going, say, 422->420, we'd
        // actually end up copying odd lines - now we'll copy even lines.
        // I dont actually think either way is 'theoretically' any better, but
        // this is a touch more logical (and a smidgen faster)
        for (y=0; y<in.height; y+=out.cy_div) 
			for (x=0; x<in.width; x+=out.cx_div) 
                copy_pel(1, x, y);

        for (y=0; y<in.height; y+=out.cy_div) 
            for (x=0; x<in.width; x+=out.cx_div) 
                copy_pel(2, x, y);
        
        if (!write_yuv()) error("Failed to write output\n");
        
        frames++;
    }

    // nothing converted, something went wrong.
    if (frames == 0) error("Failed to convert any frames\n");
    
    return 0;
}

