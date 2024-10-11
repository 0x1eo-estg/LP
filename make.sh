WARN='\033[1;31m' #red - light
SUCC='\033[1;32m' #green - light

make #call Makefile
echo -e "${WARN}Deleting .o files & build dir"
rm -f ./src/*.o #remove output files
rm -rf ./build
echo -e "${SUCC}Done!"
