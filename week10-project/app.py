from cs50 import SQL
from flask import Flask, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

# Configure application
app = Flask(__name__)
# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True
# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///channels.db")
# CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NO NULL, hash TEXT NOT NULL);
# CREATE TABLE register (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, channel TEXT, points INTEGER);
# CREATE TABLE channels (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, url TEXT, categorie TEXT, votes NUMERIC, points NUMERIC);
# CREATE TABLE categories (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, categorie TEXT);
# categories: comedy, adventure, sports, cars, computers, games, travel, music, information, news


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response



# Main templkate, just return che channels page
@app.route("/")
def index():
    return channels()



# Main channels page
@app.route("/channels", methods=["GET", "POST"])
def channels():
    if request.method == "POST":
        # if a categorie is selected
        categorie=request.form.get("cat")
        # i take all the channes whit that categorie and send it to the template
        n_list=db.execute("SELECT * FROM channels WHERE categorie = ? ORDER BY points DESC, votes DESC",categorie)
        categories = db.execute("SELECT * FROM categories")
        return render_template("channels.html", channels=n_list, categories=categories)
    # if no categorie selectet i send all the chanels to the template
    categories = db.execute("SELECT * FROM categories")
    channels = db.execute("SELECT * FROM channels ORDER BY points DESC, votes DESC")
    return render_template("channels.html", channels=channels, categories=categories)



# Template to vote for a channel
@app.route("/vote", methods=["GET", "POST"])
def vote():
    if session.get("user_id") is None:
        # if there is no accound logged, we send a message, it's necessary to be logged to vote
        return message("Login required to vote")
    else:
        if request.method == "POST":
            user_id=session["user_id"]
            name=request.form.get("name")
            points=int(request.form.get("point"))
            # if there is no channels selected we send message
            if name == None:
                return message("You must select which channel you want to vote")
            # check if the user has already vote for this channel and send a message
            rows=db.execute("SELECT * FROM register WHERE user_id = ? AND channel = ?",user_id,name)
            if len(rows) == 1:
                # if the is a lane with the name means that the usar have already vote for this channel
                return message("You have already vote for this channel")
            else:
                # If there is no line found we continue with the process and add new vote
                db.execute("INSERT INTO register (user_id, channel, points) VALUES (?,?,?)",user_id,name,points)
                # we select the actual points and votes
                list = db.execute("SELECT * FROM channels WHERE name=?", name)
                # adding one vote more
                Lvotes = float(list[0]["votes"])
                Nvotes = Lvotes+1
                # calculate new puntiation with only two decimals
                Lpoint = (float(list[0]["points"]))
                Npoint = ((Lpoint*Lvotes)+points)/Nvotes
                Npoint = round(Npoint,1)
                # add new puntuation and votes to the channel
                db.execute("UPDATE channels SET points=?, votes=? WHERE name = ?",Npoint,Nvotes,name)
                # return to chaneels voted
                register=db.execute("SELECT * FROM register WHERE user_id =?",user_id)
                return render_template("like.html", register=register)
        # render votes template with the channels
        cha = db.execute("SELECT name FROM channels ORDER BY name")
        channels=[]
        for i in cha:
            channels.append(i['name'])
        return render_template("vote.html", channels=channels)


@app.route("/like")
def like():
    if session.get("user_id") is None:
        return message("Login required to view your list")
    else:
        # return to chaneels voted
        user_id=session["user_id"]
        register=db.execute("SELECT * FROM register WHERE user_id =?",user_id)
        return render_template("like.html", register=register)


# Template to add information to the channels list (private template, just to facilitate the work)
@app.route("/put", methods=["GET", "POST"])
def put():
    if request.method == "POST":
        # take the dates and insert intothe table
        # without checkit couse is made by myself (i will write correctly or modified if needed)
        name=request.form.get("name")
        url=request.form.get("url")
        categorie=request.form.get("categorie")
        votes=request.form.get("votes")
        points=request.form.get("points")
        db.execute ("INSERT INTO channels (name, url, categorie, votes, points) VALUES (?,?,?,?,?)",name,url,categorie,votes,points)
        return redirect("/put")
    cat = db.execute("SELECT * FROM categories")
    categories=[]
    for i in cat:
        categories.append(i['categorie'])
    return render_template("put.html", categories=categories)



# template to show infomation about the page
@app.route("/about")
def about():
    return render_template("about.html")



# Template to show messages
def message(message):
    return render_template("message.html", message=message)






# All session / login / logout apps


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        # If there is no name i send a message
        if request.form.get("username") == '':
            return message("Username is requiered")
        # if there is no password i send a message
        if request.form.get("password") == '':
            return message("Password is requiered")
        # If the password dous not match with the repeated
        if not request.form.get("password") == request.form.get("confirmation"):
            return message("Password does not match")
        # if the user already exist i send a message
        users = db.execute ("SELECT username FROM users")
        for user in users:
            if user["username"] == request.form.get("username"):
                return message("User not available")
        # I made a hash from the password and save it to the table
        user=request.form.get("username")
        passw=generate_password_hash(request.form.get("password"),method='pbkdf2:sha256', salt_length=8)
        db.execute ("INSERT INTO users (username, hash) VALUES (?,?)",user,passw)
        # automatic login
        login()
        # return to the main page
        return redirect("/")
    else:
        return render_template("/register.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    session.clear()
    if request.method == "POST":
        # If there is no name i send a message
        if not request.form.get("username"):
            return message("Username is requiered")
        # if there is no password i send a message
        elif not request.form.get("password"):
            return message("Password is requiered")
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        # Check if the name exist and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return message("Invalid username or password")
        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    # Delete any user_id
    session.clear()
    # Redirect user to main page
    return redirect("/")