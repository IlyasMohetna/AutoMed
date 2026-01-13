FROM node:20-alpine

WORKDIR /app

# Changer le propriétaire du dossier de travail pour l'utilisateur node (uid 1000)
RUN chown -R node:node /app

# Passer à l'utilisateur node (qui a déjà l'uid 1000)
USER node

# Installation des dépendances en premier (cache Docker)
COPY --chown=node:node package.json ./
RUN npm install

# Copie du reste des fichiers
COPY --chown=node:node . .

# Exposition du port
EXPOSE 3000

# Commande de démarrage (Vite dev server avec hot-reload)
CMD ["npm", "run", "dev"]
