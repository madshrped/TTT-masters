bear -- g++ src/*.cpp \
  -I./src/include \
  -Wl,-Bstatic -lraylib -lglfw3 \
  -Wl,-Bdynamic -lGL -lGLdispatch -lm -lpthread -ldl -lrt \
  -g \
  -o TTT-masters
