# Jetpack

## Description

Jetpack est un remake multijoueur du jeu Jetpack Joyride. Ce projet consiste à recréer le gameplay classique en ajoutant une fonctionnalité multijoueur via un serveur réseau développé en C++.

Le serveur gère les connexions multiples, la synchronisation des joueurs et le traitement des commandes réseau.

## Fonctionnalités

- Mode multijoueur en temps réel
- Gestion des connexions clients via `poll`
- Synchronisation des positions des joueurs
- Commandes réseau pour contrôler le jetpack et envoyer la position
- Client graphique pour l’affichage et le contrôle du joueur

## Technologies

- C++17
- Sockets réseau Unix avec `poll`
- Bibliothèque graphique `SFML`
- Multithreading pour séparer réseau et rendu

## Installation

Compiler le serveur et le client :

```bash
make
```

## Utilisation

1. Lancer le serveur :

```bash
./jetpack_server [port]
```

2. Lancer un ou plusieurs clients :

```bash
./jetpack_client -h [ip] -p [port]
```

## Organisation du projet

- `server/` : code source du serveur
- `client/` : code source du client
- `doc.txt` : documentation du serveur

## Auteur

Elias Wach — elias.wach@epitech.eu — Étudiant Epitech<br>
Theotime Collier — theotime.collier@epitech.eu — Étudiant Epitech<br>
Corentin Pierrat — corentin.pierrat@epitech.eu — Étudiant Epitech
