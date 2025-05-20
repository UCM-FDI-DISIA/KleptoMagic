# 🧙🏻‍♂️ KLEPTOMAGIC 🧙🏻‍♂️
---
## Equipo de desarrollo: Team 2
### Integrantes:
- Gabriel Barroso Bueno
- Zhuokai Zhu
- Sergio Yago Juárez
- Javier Cáceres
- Sergio Croche Trigo
- Daniil Sosiedka
- Mauro Martínez
- Diego Martín García
- Davide Chen
## *Game Design Document*
## 📋 Índice
1. [Resumen](#1-resumen)
   - 1.1 [Descripción](#11-descripción)
   - 1.2 [Género](#12-género)
   - 1.3 [Público objetivo](#13-público-objetivo)
   - 1.4 [Plataforma](#14-plataforma)
2. [Mecánicas](#2-mecánicas)
   - 3.1 [Controles](#21-controles)
   - 3.2 [Movimientos](#22-movimientos)
   - 3.3 [Personajes](#23-personajes)
   - 3.4 [Estadísticas del personaje](#24-estadísticas-del-personaje)
   - 3.5 [Armas](#25-armas)
   - 3.6 [Objetos](#26-objetos)
   - 3.7 [Piso](#27-piso)
   - 3.8 [Enemigos](#28-enemigos)
3. [UI](#3-ui)
   - 3.1 [Menús](#31-menús)
   - 3.2 [Elementos](#32-elementos)
4. [Estética](#4-estética)
5. [Experiencia de juego](#5-experiencia-de-juego)
6. [Referencias](#6-referencias)
7. [Testing](#7-testing)
## 1. Resumen
### 1.1 Descripción
### 1.2 Género
Roguelike, acción, top-down.
### 1.3 Público objetivo
Mayores de 10 años, amantes de los juegos roguelike y acción.
### 1.4 Plataforma
PC Windows 10 y superior.
## 2. Mecánicas
### 2.1 Controles
El personaje se mueve con el stick izquierdo del GamePad o con WASD.  
Para apuntar se usa el stick derecho o con el ratón.  
Con el click izquierdo se ataca a la dirección que se ha apuntado.  
### 2.2 Movimientos
El usuario puede controlar el personaje en ocho direcciones: arriba, abajo, derecha, izquierda y los diagonales.
### 2.3 Personajes
Existen cuatro personajes para elegir:
| Personaje     | Imagen                | Descripción     |
|---------------|-----------------------|-----------------|
| Knight        | ![Knight](KleptoMagic_project/resources/images/knight.png)  |  Ataques a melé, cuerpo a cuerpo |
| Rogue         | ![Rogue](KleptoMagic_project/resources/images/rogue.png) | Ataques a distancia, con una velocidad medio alto, pero daño medio |
| Hunter        | ![Hunter](KleptoMagic_project/resources/images/hunter.png) | Ataques a media distancia, con una velocidad bajo, pero alto en daño de ataque  |
| Alchemist     | ![Alchemist](KleptoMagic_project/resources/images/alchemist.png) | Ataques a distancia, con una velocidad media, pero su daño es en área |
### 2.4 Estadísticas del personaje
El personaje tiene los siguientes estadísticas, que se pueden mejorar a lo largo del juego:
- Vida.
- Escudo.
- Velocidad de movimiento.
- Daño de ataque.
- Velocidad de ataque.
### 2.5 Armas
Cada personaje tienen sus propias armas:  
![Armas](KleptoMagic_project/resources/images/weapon_sprites.png)
### 2.6 Objetos
### 2.7 Piso
El juego está compuesto por un piso con distintas salas.   
Las salas están hechas con generación procedural, de esta forma, cada partida que juegas, existe un piso diferente.
### 2.8 Enemigos
Existen varios enemigos peligrosos:
| Enemigo     | Imagen                | Descripción     |
|---------------|-----------------------|-----------------|
| Slime        | ![Knight](KleptoMagic_project/resources/images/slime_sprites.png)  |  Normalmente se encuentran en cantidad, y se mueven lentamente hacia ti |
| Living Armor         | ![Rogue](KleptoMagic_project/resources/images/armor_sprites.png) | Solo se mueve en cuatro direcciones, los diagonales no. Para derrotarlo hay que atacar por la espalda |
| Ghost        |                                                                         | Se puede volver invisible, para localizar su posición podemos ver la sombra que marca en el suelo  |
| Undead Archer     |                                                        | Enemigo que ataca a larga distancia |
| Necromancer     |                                                        | Enemigo que está protegido por un escudo, a su vez invoca a 3 enemigos. Para hacerle daña hay que derrotar los enemigos que invoca |
| Boss Wizard     |                                                        | Jefe final, dispara varios proyectiles, de distintas maneras |
## 3. UI
### 3.1 Menús
- Menú principal:   
![screenshoot](KleptoMagic_project/resources/images/mainMenuGithub.png)
- Menú de selección:    
![screenshot](KleptoMagic_project/resources/images/selectMenuGithub.png)
- Menú pausa:   
![screenshot](KleptoMagic_project/resources/images/pauseMenuGithub.png)
### 3.2 Elementos
| Elemento     | Imagen                |
|---------------|-----------------------|
| Vidas        | ![vidas](KleptoMagic_project/resources/images/live.png)  |
## 4. Estética
## 5. Experiencia de juego
## 6. Referencias
- The Binding of Isaac.
- Enter the Gungeon.
- Soul Knight.
- Wizard of Legend.
## 7. Testing
