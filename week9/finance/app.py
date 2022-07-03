import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response



@app.route("/")
@login_required
def index():
    # en la pagina principal muestro siempre el porfolio actualizado
    """Show portfolio of stocks"""
    # busco el id del usuario para coger su info de las tablas generales
    user_id=session["user_id"]
    cassh = db.execute ("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cassh[0]["cash"]
    tvalue = cash
    #saco el dinero total que le queda

    # paso por la tabla de compras y la paso a una lista añaniendo datos k me interesan
    register = db.execute("SELECT * FROM current WHERE user_id = ?", user_id)
    for st in register:
        pp = lookup(st["symbol"])
        # suta total de precio comprado de todas unidades
        st["STprice"] = st["price"]*st["quantity"]
        # current price, busco el precio actual
        st["Cprice"] = pp.get('price')
        tvalue += st["STprice"]
    # redondeo decimales, añado multiplicacion de precioXcantidad y actualizo precios de AHORA

    # envio esta nueva lista junto con capital dispoible y el total de todo(capital + stonks)
    return render_template("index.html", register=register, cash=cash, tvalue=tvalue)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # si no ecribe nada devuelvo error
        if request.form.get("shares") == '':
            return apology("Introduce Shares", 400)
        if request.form.get("symbol") == '':
            return apology("Introduce Stonk", 400)
        # con el sybolo saco el nombre actual de accion
        symbol = request.form.get("symbol")
        stonk = lookup(symbol)
        # si no existe devuelvo error
        if stonk == None:
            return apology("Stonk not found", 400)

        name = stonk["name"]
        number = request.form.get("shares")
        # si no es numero entero devuelvo error
        if not number.isdigit():
            return apology("You cannot purchase partial shares", 400)
        # si la cantidad de compra no es mayor de 0 devuelvo error
        number = float(number)
        if not number>0:
            return apology("Quantity incorrect", 400)

        # busco el precio de accion y si no tiene suficiente dinero envio error
        price = stonk.get('price')
        cassh = db.execute ("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        cash = cassh[0]["cash"]
        if cash < price*number:
            return apology("You need more money to buy", 400)

        #si continuo, le resto el dinero, y actualizo tabla de compras
        cash -= price*number
        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])

        # esta seria las tabla adicionales creada en SQTlite, una para TODAS las trasacciones y otra para lo ACTUAL que se ira actualizando
        # CREATE TABLE current (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, symbol TEXT, name TEXT, price NUMERIC, quantity NUMERIC);

        #actualiza la lista actual, pero CUIDADO porque no tiene en cuenta el precio nuevo y lo mezcla con el anterior
#        symbols=db.execute("SELECT symbol FROM current WHERE user_id = ?", session["user_id"])
#        for i in symbols:
#            if i['symbol'] == symbol:
#                actual = db.execute("SELECT quantity FROM current WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
#                suma = actual[0]['quantity']
#                suma=number+suma
#                db.execute("UPDATE current SET quantity = ? WHERE user_id=? AND symbol =?", suma, session["user_id"], symbol)
#            else:
        db.execute("INSERT INTO current (user_id, symbol, name, price, quantity) VALUES (?,?,?,?,?)", session["user_id"], symbol, name, price, number)

        # CREATE TABLE register (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER, symbol TEXT, name TEXT, price NUMERIC, quantity NUMERIC, type TEXT, time DATETIME);
        time = datetime.today().strftime('%Y-%m-%d %H:%M:%S')
        db.execute("INSERT INTO register (user_id, symbol, name, price, quantity, time, type) VALUES (?,?,?,?,?,?,?)", session["user_id"], symbol, name, price, number, time, "buy")
        flash("Bougth!")
        return redirect("/")

    else:
        return render_template("buy.html")

@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    # nuevo metodo para ñadir dinero a la cuenta del usuario
    if request.method == "POST":
        money = float(request.form.get("money"))
        mactual = db.execute ("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        actual = mactual[0]["cash"] + money
        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=actual, user_id=session["user_id"])
        return redirect("/")

    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id=session["user_id"]
    register = db.execute("SELECT * FROM register WHERE user_id = ?", user_id)
    #paso la lista de registros
    return render_template("history.html", register=register)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")



@app.route("/logout")
def logout():
    """Log user out"""
    # Forget any user_id
    session.clear()
    # Redirect user to login form
    return redirect("/")



@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # busco la lista de nombre, precio etc.. si no existe sera None y devuelvo error
    if request.method == "POST":
        if request.form.get("symbol") == '':
            return apology("Stonk not found", 400)
        stonk = lookup(request.form.get("symbol"))
        if stonk == None:
            return apology("Stonk not found", 400)
        # si existe, paso la lista con todos los datos al HTML y lo puestro alli
        return render_template("quoted.html", stonk=stonk)

    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    # si publico con boton paso los datos a SQL y voy a la pantalla login
    if request.method == "POST":
        # si no hay usuario o contra escrito ERROR
        if request.form.get("username") == '':
            return apology("User is empty", 400)
        if request.form.get("password") == '':
            return apology("User is empty", 400)
        # si la contraseña no es igual a confirmacion ERROR
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("Password does not match", 400)
        # si el usuario ya existe ERROR
        users = db.execute ("SELECT username FROM users")
        for user in users:
            if user["username"] == request.form.get("username"):
                return apology("User not available", 400)

        # genero un hash de contraseña y guardo usuario y hash en la lista
        user=request.form.get("username")
        passw=generate_password_hash(request.form.get("password"),method='pbkdf2:sha256', salt_length=8)
        db.execute ("INSERT INTO users (username, hash) VALUES (?,?)",user,passw)
        login()
        flash("Registered!")
        return redirect("/")

    # mientras no envio nada me mantengo en la pagina register
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        user_id=session["user_id"]

        # con el sybolo saco el nombre actual de accion
        symbol = request.form.get("symbol")
        stonk = lookup(symbol)
        name = stonk["name"]
        # si no existe devuelvo error
        if stonk == None:
            return apology("Introduce Stonk", 400)
        if request.form.get("shares") == '':
            return apology("Introduce Shares", 400)
        number = float(request.form.get("shares"))
        # si la cantidad de compra no es mayor de 0 devuelvo error
        if not number>0:
            return apology("Introduce quantity", 400)
        # si no tiene suficiente cantidad devuelvo error
        tti = db.execute("SELECT quantity FROM current WHERE user_id = ? AND symbol = ?", user_id, symbol)
        tiene = float(tti[0]['quantity'])
        if number > tiene:
            return apology("Shares not enought", 400)

        # le quito las acciones vendidas, si las vende todas elimino la fila
        if tiene == number:
            db.execute("DELETE FROM current WHERE user_id = ? AND symbol = ?", user_id, symbol)
        elif tiene > number:
            Nnumber = tiene-number
            db.execute("UPDATE current SET quantity = ? WHERE user_id = ? AND symbol = ?", Nnumber, user_id, symbol)

        # busco el precio de accion y se lo sumo a lo que ya tiene
        price = stonk.get('price')
        Tprice = price*number
        cassh = db.execute ("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        cash = cassh[0]["cash"] + Tprice
        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])


        #actualizar transacciones
        time = datetime.today().strftime('%Y-%m-%d %H:%M:%S')
        db.execute("INSERT INTO register (user_id, symbol, name, price, quantity, time, type) VALUES (?,?,?,?,?,?,?)", session["user_id"], symbol, name, price, number, time, "sell")

        flash("Sold!")
        return redirect("/")

    else:
        user_id=session["user_id"]
        lista = db.execute("SELECT symbol FROM current WHERE user_id = ? ", user_id)
        data=[]
        for i in lista:
            data.append(i['symbol'])
        return render_template("sell.html", stonks=data)
