# TP3_Web


Lien Front End  = https://rotten-frontend.herokuapp.com <br>
Lien Back End = https://vuetp3.herokuapp.com/


<h1>Code Source </h1>

FrontEnd : 

1) npm -i 

2) npm run serve

BackEnd : 

Modifier le .env pour vos info de BD dans le folder backend<br>
Ensuite entrer les commandes suivantes:

1) composer update --no-scripts

2) php artisan migrate ( la premiere fois ) 

3) php artisan db:seed ( la premiere fois ) 

4) php artisan serve

-----------------------------------------------------------------

pour se creer un user avec l'api 

dans postman : 

1) Headers = Accept application/json

2) Body = raw Json

users/register POST:

    {
        "login" : "admin",
        "password" : "admin2002",
        "email" : "admin@gmail.com",
        "last_name" : "admin",
        "first_name" : "admin",
        "role_id" : "1"
    }

role_id 1 = admin 
role_id 2 = user


Voici les Routes du backend

|ROUTE | ACTION | DESCRIPTION |
|------|--------|-------------|
| films                    | GET        | Consultation des films (sans critiques et sans acteurs)   |
| films/actors/{id}        | GET        | Consultation de tous les acteurs d’un certain film        |
| films/critics/{id}       | GET        | Consultation d’un certain film avec ces critiques         |
| films/search             | GET        | Recherche de films                                        |
| films                    | POST       | Ajout d’un film (seulement si admin)                      |
| films/{id}               | DELETE     | Suppression d’un film (seulement si admin)                |
| users/register           | POST       | Ajout d’un nouveau user                                   |
| users/login              | POST       | Connexion d’un user                                       |
| users/{id}               | GET        | Consultation des informations d’un certain user           |
| users/logout             | POST       | Déconnexion d’un user                                     |
| users                    | PUT        | Modification d’un user existant                           |
| users/password           | PUT        | Modification du Mot de Passe                              |
| critics                  | POST       | Ajout d’une critique (seulement si membre connecté)       |

Exemple de requete json avec Postman:

films POST:
        
    {            
        "title": "Raph Contre Attack",
        "release_year": "2006",
        "length": 48,
        "description": "A Astounding Epistle of a Database Administrator And a Explorer who must Find a Car in Ancient China",
        "rating": "G",
        "language_id": 1,
        "special_features": "Trailers,Deleted Scenes",
        "image": "null"
    }

users/register POST:

    {
        "login" : "admin",
        "password" : "admin2002",
        "email" : "admin@gmail.com",
        "last_name" : "admin",
        "first_name" : "admin",
        "role_id" : "1"
    }

users/login POST:

    {
        "login" : "admin",
        "password":"admin2002"   
    }

films/search GET:

    {
    "keywords":"A",
    "max_length" : "150",
    "rating" : "G"
    }


