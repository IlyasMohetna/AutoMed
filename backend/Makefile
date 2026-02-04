# Makefile Backend C++ - AutoMed

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src -I./include
LDFLAGS = -lpthread -lboost_system

# Dossiers
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Fichiers sources
SOURCES = $(SRC_DIR)/main.cpp
OBJECTS = $(BUILD_DIR)/main.o
EXECUTABLE = $(BIN_DIR)/automed_server

# Benchmark
BENCHMARK_SRC = $(SRC_DIR)/benchmark_main.cpp
BENCHMARK_OBJ = $(BUILD_DIR)/benchmark_main.o
BENCHMARK_EXE = $(BIN_DIR)/automed_benchmark

# Couleurs pour l'affichage
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

all: $(EXECUTABLE) $(BENCHMARK_EXE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp | $(BUILD_DIR)
	@echo "$(YELLOW)[Compilation]$(NC) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	@echo "$(YELLOW)[Linkage]$(NC) Création de l'exécutable..."
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(GREEN)✓ Build réussi !$(NC) Exécutable: $(EXECUTABLE)"

$(BUILD_DIR)/benchmark_main.o: $(SRC_DIR)/benchmark_main.cpp | $(BUILD_DIR)
	@echo "$(YELLOW)[Compilation]$(NC) $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BENCHMARK_EXE): $(BENCHMARK_OBJ) | $(BIN_DIR)
	@echo "$(YELLOW)[Linkage]$(NC) Création du benchmark..."
	@$(CXX) $(BENCHMARK_OBJ) -o $@ $(LDFLAGS)
	@echo "$(GREEN)✓ Benchmark compilé !$(NC) Exécutable: $(BENCHMARK_EXE)"

run: $(EXECUTABLE)
	@echo "$(GREEN)[Démarrage]$(NC) Lancement du serveur..."
	@./$(EXECUTABLE)

benchmark: $(BENCHMARK_EXE)
	@echo "$(GREEN)[Benchmark]$(NC) Lancement de l'analyse comparative..."
	@mkdir -p results
	@./$(BENCHMARK_EXE)

clean:
	@echo "$(YELLOW)[Nettoyage]$(NC) Suppression des fichiers de build..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(GREEN)✓ Nettoyage terminé$(NC)"

rebuild: clean all

.PHONY: all run clean rebuild benchmark
