###
gengrids out.i420.720x486.8bit.yuv 720 486

(Initially played 720p and 1080i from the raid server and captured these on the DVS; the raid server doesn't play out SD though)

yconvert -f i420 -F uyvy -s 720x486 ntsc.i420.720x486.8bit.yuv ntsc.uyuv.720x486.8bit.yuv

./splitUYVY (ntsc.uyuv.720x486.8bit.yuv, to generate individual UYVY frames for DVS)

Add testcards to the start of the grid sequence (for PQA temporal alignment)
(Note: if you put settings to NTSC first and then add 1080i60 clips, the DVS will cut out a NTSC section from the 1080i60 frames).

(Note: The viewer on the DVS didn't show the NTSC sequence correctly, I thought it was wrong as so started creating 10bit versions, but my 8bit version was correct, just connect DVS output to a SD monitor and then the output will look like that from the pyuv player).

(Note: yconvert stopped writing to file after 2GB had been written (only 517 frames for UYVY 1080i), so, for 1080i60, I went via the i420 -- raidServer -- DVS capture UYVY -- add testcard -- .vcap capture on PQA). I was able to yconvert both NTSC_720x486 and 1280x720p60 to UYVY and then split them into single UYVY frames, put directly onto DVS (no raidServer) -- add testcards -- .vcap capture on PQA.
