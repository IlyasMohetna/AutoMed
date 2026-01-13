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

# Couleurs pour l'affichage
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

all: $(EXECUTABLE)

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

run: $(EXECUTABLE)
	@echo "$(GREEN)[Démarrage]$(NC) Lancement du serveur..."
	@./$(EXECUTABLE)

clean:
	@echo "$(YELLOW)[Nettoyage]$(NC) Suppression des fichiers de build..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(GREEN)✓ Nettoyage terminé$(NC)"

rebuild: clean all

.PHONY: all run clean rebuild
