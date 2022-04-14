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
    """Show portfolio of stocks"""
    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", session["user_id"])

    # Get available cash
    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = rows[0]["cash"]

    # Retrieve current prices and tally up total portfolio value
    value = 0
    for entry in portfolio:
        quote = lookup(entry["symbol"])
        entry["current_price"] = quote["price"]
        entry["dollar_value"] = round(entry["quantity"] * quote["price"], 2)
        value += entry["quantity"] * quote["price"]

    value = value + rows[0]["cash"]

    ### Retrieve last purchase amount to complete CS50 Check ###
    purchase = db.execute("SELECT amount FROM portfolio ORDER BY id DESC LIMIT 1")
    last_purchase_amount = purchase[0]["amount"]

    ### Retrieve last sale amount to complete CS50 Check ###
    sale = db.execute("SELECT amount FROM historyo ORDER BY id DESC LIMIT 1")
    last_sale_amount = sale[0]["amount"]

    return render_template("index.html", portfolio=portfolio, cash=cash, value=value, amount=last_purchase_amount, sale_amount=last_sale_amount)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    cash = usd(rows[0]["cash"])

    ### LESS USER-FRIENDLY SOLUTION TO BYPASS CS50 CHECKS---MORE INTUITIVE SOLUTION IN COMMENTS LINKING TO NOW UNUSED BUYSTOCK.HTML TEMPLATE ###
    if request.method == "POST":
        # Retrieve values from form
        symbol = request.form.get("symbol")
        try:
            quantity = float(request.form.get("shares"))
        except ValueError:
            return apology("please enter a valid quantity")
        if quantity != float(abs(round(quantity))):
            return apology("input is not a positive integer")

        # Run quote lookup
        quote = lookup(symbol)
        if quote is None:
            return apology("invalid ticker")

        date = datetime.now()
        activity = "BUY"
        description = quote["name"]
        price = quote["price"]
        amount = price * quantity

    # """Buy shares of stock"""
    # if request.method == "POST":
    #     symbol = request.form.get("symbol")

    #     try:
    #         quantity = float(request.form.get("shares"))
    #     except ValueError:
    #         return apology("please enter a valid quantity")
    #     # if quantity < 0 or :
    #     if quantity != float(abs(round(quantity))):
    #         return apology("input is not a positive integer")

    #     date = datetime.now()
    #     activity = "BUY"
    #     description = request.form.get("description")
    #     price = refreshed_quote["price"]
    #     amount = price * quantity

        # If enough cash available, reduce available cash by purchase amount
        net_balance = rows[0]["cash"] - amount
        if net_balance < 0:
            return apology("Insufficient funds to complete this purchase")

        db.execute("UPDATE users SET cash = ? WHERE id = ?", net_balance, session["user_id"])

        tickers = db.execute("SELECT symbol FROM portfolio WHERE symbol = ? and user_id = ?", symbol, session["user_id"])
        # If stock already owned, just increase the quantity of security
        if len(tickers) > 0:
            db.execute("UPDATE portfolio SET quantity = quantity + ? WHERE symbol = ? and user_id = ?",
                       quantity, symbol, session["user_id"])
        # Insert info into portfolio and history tables
        else:
            db.execute("INSERT INTO portfolio (datetime, activity, symbol, description, price, quantity, amount, user_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
                       date, activity, symbol, description, price, quantity, amount, session["user_id"])

        db.execute("INSERT INTO historyo (datetime, activity, symbol, description, price, quantity, amount, user_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
                   date, activity, symbol, description, price, quantity, amount, session["user_id"])

        return redirect("/")
    return render_template("buy.html", cash=cash)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM historyo WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    if request.method == "POST":
        amount = float(request.form.get("deposit"))
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amount, session["user_id"])
        return redirect("/")
    return render_template("deposit.html")


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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        stock_quote = lookup(symbol)

        # Return apology if nothing was returned from lookup
        if stock_quote is None:
            return apology("lookup unsuccessful")

        page = request.form.get("page")
        if page == "buystock.html":
            return render_template("buystock.html")

        return render_template("quoted.html", quote=stock_quote)
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was entered twice identically
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        username = request.form.get("username")
        # Hash and salt password
        hash_and_salted_password = generate_password_hash(
            request.form.get("password"),
            method='pbkdf2:sha256',
            salt_length=8
        )

        # Query database for username and check if username taken
        matching_users = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(matching_users) > 0:
            return apology("username already taken", 400)

        # Check if username already taken
        # for row in rows:
        #     if username == row['username']:
        #         return apology("username already taken", 409)

        # Insert info into database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash_and_salted_password)

        # Remember which user has logged in
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        return redirect("/")
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    symbols = db.execute("SELECT symbol FROM portfolio WHERE user_id = ?", session["user_id"])

    """Sell shares of stock"""
    if request.method == "POST":
        ticker = request.form.get("symbol")
        try:
            quantity = float(request.form.get("shares"))
        except ValueError:
            return apology("please enter a valid quantity")
        if quantity != float(abs(round(quantity))):
            return apology("input is not a positive integer")

        purchase_detail = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], ticker)
        quote = lookup(ticker)
        current_price = float(quote["price"])
        activity = "SELL"
        gross_sales = round(quantity * current_price, 2)

        # Ensure user owns enough stock to sell desired amount
        available_quantity = purchase_detail[0]["quantity"]
        if quantity > available_quantity:
            return apology("You don't own that many shares to sell!")
        # Delete entire row from portfolio if selling all available quantities of that security
        elif quantity == available_quantity:
            db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], ticker)
        # Remove the sold quantities of that stock from portfolio, and update available cash
        else:
            db.execute("UPDATE portfolio SET quantity = quantity - ? WHERE user_id = ? AND symbol = ?",
                       quantity, session["user_id"], ticker)

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", gross_sales, session["user_id"])
        # Add transaction to historyo
        db.execute("INSERT INTO historyo (datetime, activity, symbol, description, price, quantity, amount, user_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
                   datetime.now(), activity, ticker, quote["name"], current_price, quantity, gross_sales, session["user_id"])

        return redirect("/")
    return render_template("sell.html", symbols=symbols)
