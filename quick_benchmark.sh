#!/bin/bash

# Quick benchmark runner - runs scenario 1 (normal load) by default

echo "🚀 Running AutoMed Algorithm Benchmark..."
echo ""

# Run the benchmark in Docker with scenario 1 (or custom scenario from argument)
SCENARIO=${1:-1}

docker compose exec backend bash -c "
    cd /app && \
    mkdir -p results && \
    echo '$SCENARIO' | ./bin/automed_benchmark
"

echo ""
echo "✅ Benchmark complete! Check backend/results/ for detailed reports."
