bear -- g++ src/*.cpp \
  -I./src/include \
  -L./src/lib \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
  -g \
  -o TTT-masters
