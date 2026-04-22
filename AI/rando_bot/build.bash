bear -- g++ \
  -fPIC \
  -shared \
  -I./src/include ./src/*.cpp \
  -lstdc++ \
  -lm \
  -lpthread \
  -o rando_bot.so \
  -g
