GCC = g++
TARGET = sweet_deamon


### route setting
ROOT_DIR = .
TARGET_DIR = $(ROOT_DIR)/bin
#INC_DIR = $(ROOT_DIR)/include
#LIB_DIR = $(ROOT_DIR)/libraries
#PUB_DIR = $(ROOT_DIR)/public
SOURCE_DIR = $(ROOT_DIR)/$(TARGET)
OBJ_DIR = ${ROOT_DIR}/obj


### create a "bin" directory
$(shell mkdir -p ${OBJ_DIR})
$(shell mkdir -p ${TARGET_DIR})
RM = rm -rf


### flags setting
CFLAGS = -g -O3 -Wall -DLINUX2 -D_REENTRANT -DOS_LINUX


### include setting
#SYS_INC = -I/usr/include/mysql

INC = -I$(SOURCE_DIR) -I$(SOURCE_DIR)/common/include -I$(SOURCE_DIR)/common/tinyxml


### libraries setting
SYS_LIB = -lpthread -ldl -lrt -m64

LIB = $(SYS_LIB)


### srcource code setting
SRC = $(wildcard $(SOURCE_DIR)/*.cpp) \
		$(wildcard $(SOURCE_DIR)/common/*.cpp) \
		$(wildcard $(SOURCE_DIR)/common/include/*.cpp) \
		$(wildcard $(SOURCE_DIR)/common/tinyxml/*.cpp)


### object setting
OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRC))) 


### target setting
$(TARGET_DIR)/$(TARGET):$(OBJ)
	$(GCC) $(OBJ) -o $@ $(LIB)


### generated object file

$(OBJ_DIR)/%.o:$(PUB_DIR)/common/%.cpp
	$(GCC) $(CFLAGS) -c  $< -o $@ $(INC)
$(OBJ_DIR)/%.o:$(PUB_DIR)/common/include/%.cpp
	$(GCC) $(CFLAGS) -c  $< -o $@ $(INC)
$(OBJ_DIR)/%.o:$(PUB_DIR)/common/include/%.cpp
	$(GCC) $(CFLAGS) -c  $< -o $@ $(INC)

$(OBJ_DIR)/%.o:$(SMSP_DIR)/%.cpp
	$(GCC) $(CFLAGS) -c  $< -o $@ $(INC)


### remove object
.PHONY : clean
clean :
	$(RM) $(OBJ_DIR)


