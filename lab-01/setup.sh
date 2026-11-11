#!/usr/bin/env bash
set -euo pipefail
mkdir -p data backup reports
cat > data/measurements.txt <<'EOF_DATA'
12.5 sensor-a
14.0 sensor-b
11.2 sensor-a
13.8 sensor-b
10.9 sensor-a
EOF_DATA
echo "Готово. Створено data/measurements.txt (рядків: $(wc -l < data/measurements.txt))."
