# FlexiSoft Runtime — démarrage rapide

**TEST DE LOCALISATION : README_fr.md**

FlexiSoft Runtime est une application de service autonome destinée à surveiller les contrôleurs de sécurité SICK Flexi Soft, à récupérer de manière contrôlée certains états d’erreur et à informer l’opérateur des erreurs répétées sur les entrées de sécurité.

## Objectif

L’application est prévue pour les machines où Flexi Soft signale des erreurs d’entrée de sécurité provoquées par une commutation non simultanée de capteurs de sécurité à deux canaux.

Scénario typique :

1. Un protecteur est ouvert puis fermé.
2. Les deux canaux de sécurité ne commutent pas exactement en même temps.
3. Flexi Soft signale une erreur de simultanéité d’entrée.
4. FlexiSoft Runtime détecte l’erreur.
5. L’opérateur peut confirmer une commande de réinitialisation contrôlée.
6. Si la cause était seulement un décalage temporel, l’entrée revient à OK.
7. Si le défaut physique est toujours présent, l’erreur reste active et l’opérateur est averti de nouveau.

L’application ne remplace pas la fonction de sécurité de Flexi Soft. Elle assure uniquement la surveillance, l’information de l’opérateur et l’envoi des commandes de reset configurées.

## Fonctions principales

* Communication RK512 avec SICK Flexi Soft.
* Transport série.
* Transport TCP client via convertisseur TCP-to-RS232.
* Surveillance de quatre canaux configurés au maximum.
* Une alerte commune pour plusieurs erreurs actives.
* Commande de reset contrôlée pour les canaux concernés.
* Détection d’erreur répétée.
* Alerte de commande échouée avec option de réessai.
* Indication d’état dans la zone de notification.
* Journal runtime.
* Configuration JSON.
* Compatibilité Windows XP SP3 32-bit à Windows 11.

## Installation

Dossier recommandé :

```text
C:\FlexiSoftRuntime\
```

Contenu recommandé :

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

## Premier démarrage

1. Copiez `FlexiSoftRuntime.exe` et les fichiers de configuration dans le dossier de travail.
2. Vérifiez les paramètres de communication dans `config.json`.
3. Lancez `FlexiSoftRuntime.exe`.
4. L’icône apparaît dans la zone de notification Windows.
5. Attendez que l’icône devienne verte.

## Couleurs de l’icône

| Couleur | Signification |
| --- | --- |
| Vert | Communication OK et aucune erreur configurée active. |
| Gris | Communication inactive ou transport déconnecté. |
| Jaune / clignotant | Connexion ou reconnexion en cours. |
| Rouge | Communication OK, mais au moins un canal est en erreur. |

## Alertes

* **Input Error Alert** — erreur d’entrée active.
* **Repeat Fault Alert** — la même erreur revient après une commande.
* **Command Failed Alert** — la commande de reset n’a pas été terminée correctement.

## Documentation

Documentation de service détaillée :

```text
docs\MANUAL_fr.md
```

Si la documentation localisée n’est pas disponible, l’application ouvre le fallback :

```text
docs\README.md
docs\MANUAL.md
```

## Auteur

```text
KVLab - Vladimír Kopal
vladakopal@gmail.com
```
