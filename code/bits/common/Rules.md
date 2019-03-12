# Start Game
Les joueurs choisissent leur spawn parmi une sélection donnée par map.txt
L'ordre de sélection des points de départ est défini par leur ID.

=> Une fois le choix fait, on envoie un paquet pour initialiser le modèle de tous les joueurs

# Pre-game
On place un premier régiment sur le spawn (100 unités ?).

# Real time game
## Move
Pour déplacer un régiment :
1. On sélectionne le régiment
2. On clique sur la case de destination vide ou allié
3. Le déplacement est progressif, une unité par X secondes jusqu'à la disparation du régiment de source
Si on veut diviser un régiment, on choisit juste plusieurs destinations par régiment source

## Fight
Pour attaquer un régiment :
1. On sélectionne le régiment
2. On clique sur la case de destination où se trouve un régiment adverse
3. Une bataille commence (le serveur avertit tous les joueurs) et toute les Y secondes on fait un combat entre deux unités (avec résolution aléatoire).
4. La bataille s'arrête lorsque l'un des deux régiments n'a plus d'unité (le serveur avertit tous les joueurs de la fin de la bataille)

### Note
On a tout intérêt à encercler les régiments pour créer plusieurs batailles simultanées au même endroit

## Victory
Last man stand
