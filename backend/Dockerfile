FROM gcc:latest

# Installation des dépendances (sans boost-all, juste ce qu'il faut)
RUN apt-get update && apt-get install -y \
    libboost-system-dev \
    libboost-thread-dev \
    libasio-dev \
    inotify-tools \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Installation de websocketpp (header-only) - gardé pour plus tard
RUN cd /tmp && \
    git clone https://github.com/zaphoyd/websocketpp.git && \
    cp -r websocketpp/websocketpp /usr/local/include/ && \
    rm -rf websocketpp

# Installation de Crow (REST API, header-only)
RUN cd /tmp && \
    git clone https://github.com/CrowCpp/Crow.git && \
    cp -r Crow/include/* /usr/local/include/ && \
    rm -rf Crow

# Installation de nlohmann/json (header-only)
RUN cd /tmp && \
    git clone https://github.com/nlohmann/json.git && \
    mkdir -p /usr/local/include/nlohmann && \
    cp json/single_include/nlohmann/json.hpp /usr/local/include/nlohmann/ && \
    rm -rf json

WORKDIR /app

# Script de hot-reload
COPY docker-watch.sh /usr/local/bin/docker-watch.sh
RUN chmod +x /usr/local/bin/docker-watch.sh

# Exposition du port
EXPOSE 8080

# Commande par défaut : hot-reload
CMD ["/usr/local/bin/docker-watch.sh"]
