### make file template for testing
CC = g++
CFLAGS = -std=c++11
INC_DIR = ./inc
SRC_DIR = ./src
TEST_DIR = ./test
BUILD_DIR = ./build

all: dir SiameseTracker

test: dir reducer max_pool memory_controller

dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/PE
	mkdir -p $(BUILD_DIR)/AIM


############################  Main  ############################
CPP = $(wildcard src/*.cpp src/pe/*.cpp src/AIM/*.cpp)

# AIM_CPP = associative_index_matching_unit.cpp comparator_array.cpp \
# 					priority_encoder_set.cpp priority_encoder.cpp
# PE_CPP = processing_element.cpp
TARGET = SiameseTracker

# AIM_CPP := $(addprefix AIM/,$(AIM_CPP))
# PE_CPP := $(addprefix pe/,$(PE_CPP))


EXE = $(addprefix $(BUILD_DIR)/,$(TARGET))
# SRC = $(addprefix $(SRC_DIR)/,$(CPP))
OBJ = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o, $(CPP)) 
# AIM_SRCS = $(addprefix $(SRC_DIR)/,$(AIM_CPP))
# AIM_OBJS = $(patsubst AIM/%.cpp,$(BUILD_DIR)/%.o, $(AIM_CPP))
# PE_SRCS = $(addprefix $(SRC_DIR)/,$(PE_CPP))
# PE_OBJS = $(patsubst PE/%.cpp,$(BUILD_DIR)/%.o, $(PE_CPP))

$(TARGET): $(EXE)
# $(TARGET):
# 	echo $(OBJ)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)



############################  Test  ############################

###--------------- REDUCER ---------------###
#### modify me ####
TEST_MAIN = test_reducer
CPP = reducer.cpp
TARGET = reducer
##################

EXE = $(addprefix $(BUILD_DIR)/,$(TEST_MAIN))
TEST_SRC = $(addprefix $(TEST_DIR)/,$(TEST_MAIN).cpp)
TEST_OBJ = $(patsubst %, $(BUILD_DIR)/%.o, $(TEST_MAIN)) 
SRCS = $(addprefix $(SRC_DIR)/,$(CPP))
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(CPP)) 

$(TARGET): $(EXE)

$(EXE): $(TEST_OBJ) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_OBJ): $(TEST_SRC)
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

###------------ END OF REDUCER ------------###

###--------------- MAX POOL ---------------###
#### modify me ####
TEST_MAIN = test_maxpool
CPP = max_pool.cpp
TARGET = max_pool
##################

EXE  = $(addprefix $(BUILD_DIR)/,$(TEST_MAIN))
TEST_SRC = $(addprefix $(TEST_DIR)/,$(TEST_MAIN).cpp)
TEST_OBJ = $(patsubst %, $(BUILD_DIR)/%.o, $(TEST_MAIN)) 
SRCS = $(addprefix $(SRC_DIR)/,$(CPP))
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(CPP)) 

$(TARGET): $(EXE)

$(EXE): $(TEST_OBJ) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_OBJ): $(TEST_SRC)
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

###------------ END OF MAX POOl ------------###

###--------------- MEMORY_CONTROLLER ---------------###
#### modify me ####
TEST_MAIN = test_memory
# HEADER = specs.h memory_controller.h memory.h address_config.h
# CPP = memory_controller.cpp fifo.cpp memory.cpp addr_gen.cpp
HEADER = specs.h memory_controller.h memory.h address_config_v1.h
CPP = memory_controller_v1.cpp fifo.cpp memory.cpp addr_gen_v1.cpp
TARGET = memory_controller
##################

EXE  = $(addprefix $(BUILD_DIR)/,$(TEST_MAIN))
TEST_SRC = $(addprefix $(TEST_DIR)/,$(TEST_MAIN).cpp)
TEST_OBJ = $(patsubst %, $(BUILD_DIR)/%.o, $(TEST_MAIN)) 
SRCS = $(addprefix $(SRC_DIR)/,$(CPP))
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(CPP)) 

$(TARGET): $(EXE)

$(EXE): $(TEST_OBJ) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_OBJ): $(TEST_SRC)
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

###------------ END OF MEMORY_CONTROLLER ------------###

###--------------- PE ---------------###
#### modify me ####
TEST_MAIN = test_pe
CPP = $(wildcard src/PE/*.cpp src/AIM/*.cpp)
TARGET = pe
##################

EXE  = $(addprefix $(BUILD_DIR)/PE/,$(TEST_MAIN))
TEST_SRC = $(addprefix $(TEST_DIR)/PE/,$(TEST_MAIN).cpp)
TEST_OBJ = $(patsubst %, $(BUILD_DIR)/PE/%.o, $(TEST_MAIN))
OBJS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o, $(CPP)) 

$(TARGET): dir $(EXE)

$(EXE): $(TEST_OBJ) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_OBJ): $(TEST_SRC)
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

###------------ END OF PE ------------###

###--------------- COMPRESSOR ---------------###
#### modify me ####
TEST_MAIN = test_compressor
CPP = compressor.cpp
TARGET = compressor
##################

EXE = $(addprefix $(BUILD_DIR)/,$(TEST_MAIN))
TEST_SRC = $(addprefix $(TEST_DIR)/,$(TEST_MAIN).cpp)
TEST_OBJ = $(patsubst %, $(BUILD_DIR)/%.o, $(TEST_MAIN)) 
SRCS = $(addprefix $(SRC_DIR)/,$(CPP))
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(CPP)) 

$(TARGET): $(EXE)

$(EXE): $(TEST_OBJ) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_OBJ): $(TEST_SRC)
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ -I $(INC_DIR) $(CFLAGS)

###------------ END OF COMPRESSOR ------------###

clean:
	rm -rf ./build/*
