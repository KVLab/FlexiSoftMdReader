# FlexiSoft Runtime — manuel de service

**TEST DE LOCALISATION : MANUAL_fr.md**

## 1. Vue d’ensemble

FlexiSoft Runtime est une application de service pour surveiller certains états des contrôleurs de sécurité SICK Flexi Soft via la communication RK512.

L’application est destinée aux machines où un contrôleur de sécurité peut signaler une erreur après l’ouverture et la fermeture d’un protecteur. La cause typique est une commutation non simultanée d’un capteur de sécurité magnétique à deux canaux.

FlexiSoft Runtime détecte les erreurs d’entrée configurées, informe l’opérateur et permet d’envoyer une commande de reset contrôlée. Si l’état physique de sécurité est correct, le reset peut effacer l’erreur. Si le défaut est toujours présent, l’erreur reste active et l’opérateur est averti de nouveau.

FlexiSoft Runtime n’est pas un contrôleur de sécurité et ne remplace aucune fonction de sécurité de la machine.

## 2. Fichiers de l’application

Dossier recommandé :

```text
C:\FlexiSoftRuntime\
```

Structure typique :

```text
FlexiSoftRuntime.exe
config.json
languages.json
version.json
runtime_state.json
flexi_runtime.log
docs\README.md
docs\README_fr.md
docs\MANUAL.md
docs\MANUAL_fr.md
fonts\...
```

## 3. Icône tray

L’application fonctionne dans la zone de notification Windows.

| Couleur | Signification |
| --- | --- |
| Vert | Communication OK, aucune erreur configurée active. |
| Gris | Aucune communication active. |
| Jaune / clignotant | Ouverture du transport ou reconnexion. |
| Rouge | Au moins un canal configuré est en erreur. |

## 4. Menu tray

Le menu affiche l’état des canaux et les commandes de base.

`Send CHx command` met en file une commande de reset pour le canal sélectionné. La commande est traitée par le thread de communication runtime.

`Reload config` recharge la configuration et redémarre la communication.

`Reconnect` arrête le transport actuel et démarre à nouveau la communication.

`Language` change la langue active de l’interface.

`About` ouvre la fenêtre À propos.

## 5. Alertes

### 5.1 Active Input Error Alert

Affichée lorsqu’un ou plusieurs canaux configurés signalent une erreur.

| Bouton | Action |
| --- | --- |
| YES | Envoyer une commande commune pour toutes les erreurs listées. |
| NO | Fermer l’alerte sans commande. |

### 5.2 Repeat Fault Alert

Affichée lorsqu’une erreur d’entrée revient plusieurs fois après une commande.

Causes possibles :

* protecteur mal fermé,
* un canal du capteur à deux canaux est manquant,
* mauvais alignement du capteur,
* problème de câblage,
* défaut réel à ne pas contourner.

### 5.3 Command Failed Alert

Affichée lorsque la commande n’a pas été terminée correctement.

La commande est réussie seulement après acceptation du telegram ON, expiration du pulse time et acceptation du telegram OFF.

## 6. Configuration

Le fichier principal est actuellement :

```text
config.json
```

Le déplacement des fichiers JSON vers `conf/` est prévu ensuite.

## 7. transport

`transport` sélectionne la section `network` ou `serial`.

## 8. serial

Le transport série utilise COM port, baudrate, parity, stop bits et timeout. Valeur testée : `115200`.

## 9. network

Le mode `tcp_client` utilise l’adresse IP et le port du convertisseur TCP-to-RS232.

## 10. rk512

La section `rk512` définit device id, reply id et token telegram.

## 11. poll

La section `poll` définit la période de lecture, le bloc lu et la taille des données.

## 12. inputs

Chaque input décrit une entrée de sécurité logique. Si le canal est activé et que `ok_bit` vaut false, le canal est traité comme erreur.

## 13. output

`output` décrit le bit de commande utilisé pendant le reset pulse.

## 14. repeat_fault

`repeat_fault` définit le nombre de répétitions, la fenêtre temporelle et le texte de l’erreur répétée.

## 15. ui

La section `ui` contient le tooltip de l’icône tray.

## 16. logging

Le journal runtime est toujours écrit en anglais.

## 17. debug

`force_command_fail` est réservé au test de Command Failed Alert.

## 18. Localisation

Les textes UI sont chargés depuis `languages.json`.

La documentation est ouverte depuis `docs/` :

```text
docs\README_<lang>.md -> docs\README.md
docs\MANUAL_<lang>.md -> docs\MANUAL.md
```

L’ukrainien utilise le suffixe `uk`. `ua` n’est pas pris en charge.

## 19. version.json

`version.json` contient le produit, la version, la description, l’auteur, l’email et la date de build.

## 20. Dépannage

Si l’icône reste grise, vérifier le port, le convertisseur TCP, l’adresse IP, l’alimentation de Flexi Soft et le câblage.

Si l’icône est rouge, au moins un canal configuré signale une erreur.

Si l’alerte revient, le défaut physique est probablement toujours présent.

## 21. Notes de sécurité

FlexiSoft Runtime n’est pas un dispositif de sécurité et ne doit pas être utilisé pour contourner les fonctions de sécurité.

## 22. Support

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
