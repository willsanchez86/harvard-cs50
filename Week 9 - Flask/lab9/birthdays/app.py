import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_sqlalchemy import SQLAlchemy

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
app.config['SQLALCHEMY_DATABASE_URI'] = "sqlite:///birthdays.db"
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
# db = SQL("sqlite:///birthdays.db")

db = SQLAlchemy(app)

# Create table in DB
class Birthday(db.Model):
    __tablename__ = "birthdays"
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(250), nullable=False)
    month = db.Column(db.Integer, nullable=False)
    day = db.Column(db.Integer, nullable=False)

db.create_all()

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    birthdays = Birthday.query.all()
    if request.method == "POST":

        # Add the user's entry into the database
        new_birthday = Birthday(
            name = request.form.get("name"),
            month = request.form.get("month"),
            day = request.form.get("day")
        )
        db.session.add(new_birthday)
        db.session.commit()
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html

        return render_template("index.html", all_birthdays=birthdays)

if __name__ == "__main__":
    app.run()
