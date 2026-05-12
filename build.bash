bear -- g++ src/*.cpp \
  -I./src/include \
  -Wl,-Bdynamic -lGL -lGLdispatch -lm -lpthread -ldl -lrt \
  -lraylib \
  -g \
  -o TTT-masters
