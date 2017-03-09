BIN = dispose_worker
CXX = g++ 

CPPFLAGS = -c -g -rdynamic -m64
CFLAGS = -lpthread -ldl -lrt -m64
INCLUDE = -I./ -I../LibHelper/inc -I/usr/include/mysql \
		-I../LibHelper/redisclient -I../LibHelper/tinyxml -I../LibHelper/archive \
		-I../DTSAgent/agent -I../DTSAgent/inc
LIBRARY = -lhelper -L/usr/lib64/mysql -lmysqlclient -lcurl -lboost_system -ldtsagent

RM = rm -rf 

OBJECTS = analysis/common_function.o analysis/connectproc.o analysis/data_analysis.o analysis/datareceived.o analysis/savereprotdata.o analysis/sendcmd.o \
		business/BusinessService.o business/TBusiness.o \
		dispatch/Dispatch.o \
		protocol/RedisPool.o protocol/DtsPool.o\
		mysqlclient/DbPool.o mysqlclient/DbConnect.o mysqlclient/DbRecordset.o \
		BoxMsgPool.o BoxSlots.o disposeworkerapp.o \
		dispose_worker.o \
		Log.o

all: ${BIN}

${BIN}: ${OBJECTS}
	${CXX} -g ${OBJECTS} -o ${BIN} ${LIBRARY} ${CFLAGS}

${OBJECTS}:%.o:%.cpp
	@echo "Compiling $<"
	@${CXX} ${CPPFLAGS} ${INCLUDE} $< -o $@

.PHONY: clean
clean:
	-${RM} *.o analysis/*.o business/*.o dispatch/*.o protocol/*.o mysqlclient/*.o ${BIN}
