# YOUTUBE CHANNEL FINDER
### Video Demo:  <https://youtu.be/vukQ3bzMHns>
### Description:

###### This is a website made to discover new youtube channels if you are bored and want to watch some entertainment but youtube only recommends videos from the channels you already know and subscribed to, or channels that are very similar but want to watch other things...
###### Here you can find the channels according to the category that interests you, and ordered by the votes of the community, so you can be sure that the first ones will be the most interesting. The design is similar to youtube page
###### Home page, is the main page and you can see the ranking for all the channels from all the categories, in left side table there is the position, middle the channels name and right ride the rating accorting to all the votes, the channels are ordered according to the rating and number of votes, so if there is a tie between two channels, the channel with more votes will be the first one. By clicking in the row from any channel you can see one video of the channel
###### In categorie buttons (upper side), you can see the ranking for this specific categorie, same as the main page but just for the categorie
###### vote, here is possible to vote for a channel/account and contribute to the general ranking, but for that, you must be registered, only one vote per channel/account is possible. If someone try to vote more than one time for the same channel, a message will be showed. the ranking will be upgraded with you puntuation and you will be redirect to the page "channels you like"
###### channels you like, yo can check a table with all the channels and votes you made
###### About us, only information about the webpage
###### Log in / log out and Register, the name says the function :), nothing more to add, the password is saved as a hash
###### There is also a private page "/put" made for insert data in the table (new channels, with video and categorie...) it's only made to falicite the work of introduce information to the page
##### files
###### channels is where the SQL tables are, the SQL tables are specified also in app file as a comment
###### app is the main file, with fuctions for every template commented and one function extra used to show error or info messages
###### static folder we can find the CSS file (HTML was made without librarys) i like to show the things how i want and is this case the idea was to made a website similar to youtube page. And the ico picture and logo picture used in the tittle and about template
###### in templates folder are all the templates, one main page (index) used in all the other templates, the categorie buttons are made from a SQL table, so it's possible to add or remove categories easely
###### in vote template, it was difficult to find a solution to vote for a speficic number, no decimals or characters, and esay, so I don't want the user to write nothing, it whas difficult for me to use selectors for all numbers so i made the dessicion tu use the same number as a submit button, and print from a 'for' 1-11 loop, maybe not the best way because the usser can not check before submit, but easy and functional.

