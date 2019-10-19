
PROGRAM  := dtlsproxy

LIBDTLS_DIR := ./tinydtls
LIBDTLS     := $(LIBDTLS_DIR)/libtinydtls.a

SOURCES := main.c utils.c proxy.c keystore.c address.c backend.c client.c
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))

INCLUDES := -I. -I$(LIBDTLS_DIR)
DEFINES  :=
CFLAGS   := $(INCLUDES) $(DEFINES) -Wall -O2 -g
LFLAGS   := -L$(LIBDTLS_DIR) -ltinydtls -lev

SRC_TAG = ${TAG}
ifeq ("${SRC_TAG}","")
SRC_TAG = master
endif

all: $(LIBDTLS) $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $^ $(LFLAGS)

$(LIBDTLS):
	$(MAKE) -C $(LIBDTLS_DIR)

clean:
	rm -f $(PROGRAM) $(OBJECTS)

clean-libs:
	$(MAKE) -C $(LIBDTLS_DIR) clean

docker-build:
	docker build --build-arg SRC_TAG=${SRC_TAG} -t nbiotregistry.azurecr.io/${PROGRAM}:${SRC_TAG} .

docker-push: docker-build
	docker push nbiotregistry.azurecr.io/${PROGRAM}:${SRC_TAG}