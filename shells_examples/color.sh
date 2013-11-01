#!/bin/bash
# http://tldp.org/LDP/abs/html/colorizing.html
# http://www.bashguru.com/2010/01/shell-colors-colorizing-shell-scripts.html

# bold
$ echo -e "\033[1mChoose one of the following persons:\033[0m" 

# Underline
$ echo -e "\033[4mChoose one of the following persons:\033[0m"

# Scorethrough
$ echo -e "\033[9mChoose one of the following persons:\033[0m"

# blue on gray
$ echo -e '\E[47;34m'"\033[1mtestets\033[0m"

# yellow on gray
$ echo -e '\E[47;33m'"\033[1mtestets\033[0m"

# aqua on gray
$ echo -e '\E[47;36m'"\033[1mtestets\033[0m"

# red on gray
$ echo -e '\E[47;31m'"\033[1mtestets\033[0m"

# red on light-blue
$ echo -e '\E[46;31m'"\033[1mtestets\033[0m"

# red on navy
$ echo -e '\E[44;31m'"\033[1mtestets\033[0m"

# red on orange
$ echo -e '\E[43;31m'"\033[1mtestets\033[0m"

# red on green
$ echo -e '\E[42;31m'"\033[1mtestets\033[0m"

# red on white
$ echo -e '\E[40;31m'"\033[1mtestets\033[0m"

