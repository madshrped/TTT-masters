g++ src/*.cpp \
  -Isrc/include \
  -Wl,-Bdynamic -lGL -lGLdispatch -lm -lpthread -ldl -lrt \
  -lraylib \
  -g \
  -o TTT-masters
