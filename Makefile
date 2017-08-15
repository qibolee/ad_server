INC=./include
OUT=./out
SRC=./src

# compiler
CXX=clang++
# compile args
CXXFLAGS=-std=c++14 \
		 -Wall \
		 -I. \
		 -I./include
# link args
CXXLNKS=-lpthread \
		-lrt

# head files in ./include/
INCFILES=mlog.h \
		 string_util.h \
		 blocking_queue.h \
		 inet_sockets.h \
		 accept_thread.h \
		 process_thread.h \
		 reload_thread.h \
		 ad_data.h \
		 config_parser.h \
		 config.h \
		 json.hpp

# source files in ./src/
SRCFILES=main.cpp \
		 mlog.cpp \
		 string_util.cpp \
		 inet_sockets.cpp \
		 accept_thread.cpp \
		 process_thread.cpp \
		 reload_thread.cpp \
		 config_parser.cpp \
		 config.cpp

# substr *.h to ./include/*.h
TMP=$(INCFILES:%.h=$(INC)/%.h)
INCS=$(TMP:%.hpp=$(INC)/%.hpp)

# substr *.cpp to ./src/*.cpp
SRCS=$(SRCFILES:%.cpp=$(SRC)/%.cpp)

# substr *.cpp to ./out/*.o
OBJS=$(SRCFILES:%.cpp=$(OUT)/%.o)

# target, default make target
TARGET=$(OUT)/main

# biuld target
$(TARGET):$(OBJS)
	$(CXX) $(CXXLNKS) -o $@ $^
	@echo "make done"

# to all ./out/*.o:./src/*.cpp
$(OUT)/%.o:$(SRC)/%.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# phony target
clean:
	rm -rf $(OUT)/*.o $(TARGET)

# show var
show:
	@echo "INC: $(INC)"
	@echo "SRC: $(SRC)"
	@echo "OUT: $(OUT)"
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "CXXLNKS: $(CXXLNKS)"
	@echo "INCFILES: $(INCFILES)"
	@echo "SRCFILES: $(SRCFILES)"
	@echo "INCS: $(INCS)"
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "TARGET: $(TARGET)"




