all: build_main build_include link

build_main:
	g++ -c ./src/main.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-Iexternal/nlohmann_json/include \
		-o ./bin/main.o

build_include:
	g++ -c ./src/Component.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/Component.o

	g++ -c ./src/ColliderComponent.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/ColliderComponent.o

	g++ -c ./src/TransformComponent.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/TransformComponent.o

	g++ -c ./src/SpriteComponent.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/SpriteComponent.o

	g++ -c ./src/Entity.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/Entity.o

	g++ -c ./src/World.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/World.o
		
	g++ -c ./src/Game.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-Iexternal/nlohmann_json/include \
		-o ./bin/game.o

	g++ -c ./src/HealthComponent.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/HealthComponent.o

	g++ -c ./src/EnemyComponent.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/EnemyComponent.o

	g++ -c ./src/Event.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/Event.o

	g++ -c ./src/PlayerComponent.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/PlayerComponent.o

	g++ -c ./src/PlayerInputSystem.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/PlayerInputSystem.o

	g++ -c ./src/MovementSystem.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/MovementSystem.o

	g++ -c ./src/RenderSystem.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/RenderSystem.o

	g++ -c ./src/DamageSystem.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/DamageSystem.o

	g++ -c ./src/SpawnSystem.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/SpawnSystem.o

	g++ -c ./src/EnemyAISystem.cpp \
		-I./include \
		-Iexternal/spdlog/include \
		-Iexternal/GLM/include \
		-I./external/SDL3/include \
		-Iexternal/SDL3_Image/include \
		-o ./bin/EnemyAISystem.o

link:
	g++ ./bin/*.o \
		-Lexternal/SDL3/lib \
		-Lexternal/SDL3_Image/lib \
		-Lexternal/spdlog/lib \
		-Lexternal/GLM/lib \
		-lglm \
		-lspdlog \
		-lSDL3 -lSDL3_image -lwinmm -limm32 -lversion -lole32 -loleaut32 \
		-lsetupapi -lshell32 -luser32 -lgdi32 -luuid \
		-o ./bin/main

run:
	./bin/main.exe

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/*.exe

build_static:
	g++ -c ./src/calcualadora.cpp \
		-I./include \
		-o ./bin/calculadora.o
	ar rcs ./bin/lib/libcalculadora.lib \
		./bin/calculadora.o


build_glm_static:
	@echo "Compiling GLM implementation and creating static lib"
	@mkdir -p external/GLM/lib
	g++ -c external/GLM/include/glm/detail/glm.cpp -Iexternal/GLM/include -std=c++17 -O2 -fPIC -o external/GLM/lib/glm.o
	ar rcs external/GLM/lib/libglm.a external/GLM/lib/glm.o
	@echo "Created external/GLM/lib/libglm.a"

build_spdlog_static:
	@echo "Building spdlog static library"
	@mkdir -p external/spdlog/lib
	g++ -c external/spdlog/src/spdlog.cpp -Iexternal/spdlog/include -std=c++17 -O2 -fPIC -o external/spdlog/lib/spdlog.o
	ar rcs external/spdlog/lib/libspdlog.a external/spdlog/lib/spdlog.o
	@echo "Created external/spdlog/lib/libspdlog.a"