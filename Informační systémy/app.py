from flask import Flask, render_template, request, redirect, url_for, session
from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, FieldList, FormField
from datetime import timedelta
from datetime import datetime
from Crypto.Cipher import AES

#from flask_sqlalchemy import SQLAlchemy
from flask_mysqldb import MySQL
import MySQLdb.cursors
import re
import time


class form_ticket(FlaskForm):
    name = StringField("Meno")
    surname = StringField("Priezvisko")


class form_tickets(FlaskForm):
    tickets = FieldList(FormField(form_ticket), min_entries=3)
    submit = SubmitField("Objednať s povinnosťou platby")


app = Flask(__name__)
app.debug = True

app.jinja_env.add_extension('jinja2.ext.do')
app.config['MYSQL_HOST'] = 'eu-cdbr-west-01.cleardb.com'
app.config['MYSQL_USER'] = 'bfc6c258d56690'
app.config['MYSQL_PASSWORD'] = '85a6403a'
app.config['MYSQL_DB'] = 'heroku_2bec431032adf92'
app.secret_key = 'IISPROJECT'
mysql = MySQL(app)


@app.before_request
def make_session_permanent():
    session.permanent = True
    app.permanent_session_lifetime = timedelta(minutes=10)
    session.modified = True


@app.errorhandler(404)
def page_not_found(e):
    # note that we set the 404 status explicitly
    return render_template('404.html'), 404


@app.route('/login', methods=['GET', 'POST'])
def login():
    msg = ''
    if request.method == 'POST' and 'username' in request.form and 'password' in request.form:
        username = request.form['username']
        password = request.form['password']
        key = 'h8a1dda1xqDas7h4'
        iv = 'IssProjektivpass'
        aes = AES.new(key, AES.MODE_CFB, iv)
        secure_password = aes.encrypt(password)
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'SELECT * FROM user WHERE username = %s AND password = %s', (username, secure_password,))
        account = cursor.fetchone()
        if account:
            session['logged_in'] = True
            session['id'] = account['id_user']
            session['username'] = account['username']
            session['role'] = account['role']
            if(account['name'] is not None):
                session['name'] = account['name']
            if(account['surname'] is not None):
                session['surname'] = account['surname']
            cursor.close()
            return redirect(url_for('home'))
        else:
            cursor.close()
            msg = 'Nesprávne meno alebo heslo !'
    return render_template('login.html', msg=msg)


@ app.route('/logout')
def logout():
    session.pop('logged_in')
    session.pop('id')
    session.pop('username')
    session.pop('name')
    session.pop('surname')
    session.pop('role')
    return redirect(url_for('home'))


@ app.route('/register', methods=['GET', 'POST'])
def register():
    msg = ''
    if request.method == 'POST' and 'username' in request.form and 'password' in request.form and 'email' in request.form and 'name' in request.form and 'surname' in request.form:
        username = request.form['username']
        password = request.form['password']
        key = 'h8a1dda1xqDas7h4'
        iv = 'IssProjektivpass'
        aes = AES.new(key, AES.MODE_CFB, iv)
        secure_password = aes.encrypt(password)
        email = request.form['email']
        name = request.form['name']
        surname = request.form['surname']
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'SELECT * FROM user WHERE username = %s', (username,))
        account = cursor.fetchone()
        if account:
            msg = 'Účet s rovnakým menom už existuje!'
        elif not re.match(r'[^@]+@[^@]+\.[^@]+', email):
            msg = 'Neplatný email!'
        elif not re.match(r'^[a-zA-Z0-9]+$', username):
            msg = 'Použivateĺské meno nemôže obsahovať špeciálne znaky !'
        elif not re.match(r'^[a-zA-ZťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', name):
            msg = 'Meno nemôže obsahovať špeciálne znaky !'
        elif not re.match(r'^[a-zA-ZťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,.]+$', surname):
            msg = 'Meno nemôže obsahovať špeciálne znaky !'
        elif not username or not password or not email or not name or not surname:
            msg = 'Prosím vyplňte všetky políčka !'
        else:
            cursor.execute(
                'INSERT INTO user VALUES (NULL,%s,%s,%s, %s, %s,"user")', (name, surname, email, username, secure_password))
            mysql.connection.commit()
            cursor.close()
            msg = 'Registrácia sa podarila !'
    elif request.method == 'POST':
        msg = 'Prosím vyplňte všetky políčka !'
    return render_template('register.html', msg=msg)


@ app.route('/')
def home():
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('SELECT * FROM conference ')
    user = ''
    conferences = cursor.fetchall()

    if 'logged_in' in session:
        if("name" in session):
            if(session['name'] is not None):  # creator has to have his name saved
                user = session['name']
        else:
            user = session['username']

        cursor.close()
        return render_template('home.html', username=user, conferences=conferences)
    else:
        cursor.close()
        return render_template('home.html', username='none', conferences=conferences)


@ app.route('/profile')
def profile():
    # Check if user is logged_in
    if 'logged_in' in session:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM user WHERE id_user = %s',
                       (session['id'],))
        account = cursor.fetchone()
        pw = account['password']
        key = 'h8a1dda1xqDas7h4'
        iv = 'IssProjektivpass'
        aes = AES.new(key, AES.MODE_CFB, iv)
        secure_password = aes.decrypt(pw)
        secure_password = secure_password.decode('utf-8')
        account['password'] = secure_password
        cursor.close()
        return render_template('profile.html', account=account)
    # User is not logged_in
    return redirect(url_for('login'))


@ app.route('/edit_profile/<edit>', methods=['GET', 'POST'])
def edit_profile(edit):
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    msg = ''
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('SELECT * FROM user WHERE id_user = %s',
                   (session['id'],))
    account = cursor.fetchone()
    if request.method == 'POST' and 'username' in request.form and 'password' in request.form and 'email' in request.form:
        username = request.form['username']
        password = request.form['password']
        key = 'h8a1dda1xqDas7h4'
        iv = 'IssProjektivpass'
        aes = AES.new(key, AES.MODE_CFB, iv)
        secure_password = aes.encrypt(password)
        email = request.form['email']
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        if not re.match(r'[^@]+@[^@]+\.[^@]+', email):
            msg = 'Neplatná adresa!'
            cursor.close()
            return render_template('profile.html', account=account, edit='True', msg=msg)
        elif not re.match(r'^[a-zA-Z0-9]+$', username):
            msg = 'Username môže obsahovať iba písmena a čísla'
            cursor.close()
            return render_template('profile.html', account=account, edit='True', msg=msg)
        elif not username or not password or not email:
            msg = 'Prosím vyplňte všetky políčka !'
            cursor.close()
            return render_template('profile.html', account=account, edit='True', msg=msg)
        else:
            cursor.execute(
                'UPDATE user SET username= %s ,password= %s,email= %s WHERE id_user=%s', (username, secure_password, email, session['id'],))
            mysql.connection.commit()
            session['username'] = username
            msq = 'Zmena údajov prebehla úspešne'
        if(edit == 'False'):
            cursor.close()
            return redirect(url_for('profile'))
    elif request.method == 'POST':
        msg = 'Prosím vyplňte všetky políčka !'

    pw = account['password']
    key = 'h8a1dda1xqDas7h4'
    iv = 'IssProjektivpass'
    aes = AES.new(key, AES.MODE_CFB, iv)
    secure_password = aes.decrypt(pw)
    secure_password = secure_password.decode('utf-8')
    account['password'] = secure_password
    cursor.close()
    return render_template('profile.html', account=account, edit=edit, msg=msg)


@app.route('/edit_users')
def edit_users():
    msg = ''
    if 'logged_in' in session:
        if session["role"] == "admin":
            cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
            cursor.execute('SELECT * FROM user where role = "user" ')
            users = cursor.fetchall()
            cursor.close()
            return render_template('edit_users.html', users=users, msg=msg, find=False)
        else:
            return redirect(url_for('home'))
    else:
        return redirect(url_for('login'))


@app.route('/edit_users_search', methods=['GET', 'POST'])
def edit_users_search():
    msg = ''
    if 'logged_in' in session:
        if session["role"] == "admin":
            cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
            cursor.execute('SELECT * FROM user where role = "user" ')
            users = cursor.fetchall()
            if request.method == "POST" and 'find_username' in request.form:
                username = request.form['find_username']
                cursor.execute(
                    'SELECT * FROM user WHERE username = %s', (username,))
                result = cursor.fetchone()
                if result is not None:
                    cursor.close()
                    return render_template('edit_users.html', users=result, msg=msg, find=True)
                else:
                    cursor.close()
                    msg = 'Uživateľ s daným menom už existuje'
                    return render_template('edit_users.html', users=users, msg=msg, find=False)
            else:
                cursor.close()
                return render_template('edit_users.html', users=users, msg=msg, find=False)
        else:
            return redirect(url_for('home'))
    else:
        return redirect(url_for('login'))


@app.route('/go_to_edit_selected_user_page/<username>')
def go_to_edit_selected_user_page(username):
    if 'logged_in' in session:
        if session["role"] == "admin":
            cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
            cursor.execute(
                'SELECT * FROM user where username = %s ', (username,))
            user = cursor.fetchone()
            msg = ''
            cursor.close()
            pw = user['password']
            key = 'h8a1dda1xqDas7h4'
            iv = 'IssProjektivpass'
            aes = AES.new(key, AES.MODE_CFB, iv)
            secure_password = aes.decrypt(pw)
            secure_password = secure_password.decode('utf-8')
            user['password'] = secure_password
            return render_template('edit_selected_user.html', account=user, msg=msg)
        else:
            return redirect(url_for('home'))
    else:
        return redirect(url_for('login'))


@app.route('/edit_selected_user/<username>', methods=['GET', 'POST'])
def edit_selected_user(username):
    if 'logged_in' in session:
        if session["role"] == "admin":
            cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
            if request.method == 'POST' and 'username' in request.form and 'password' in request.form and 'email' in request.form and 'name' in request.form and 'surname' in request.form:
                new_username = request.form['username']
                new_password = request.form['password']
                new_email = request.form['email']
                new_name = request.form['name']
                new_surname = request.form['surname']
                key = 'h8a1dda1xqDas7h4'
                iv = 'IssProjektivpass'
                aes = AES.new(key, AES.MODE_CFB, iv)
                secure_password = aes.encrypt(new_password)
                edit_succes = False
                if not re.match(r'[^@]+@[^@]+\.[^@]+', new_email):
                    msg = 'Neplatná email adresa!'
                elif not re.match(r'^[a-zA-Z0-9]+$', new_username):
                    msg = 'Uživateľské meno môže obsahovať iba písmena a čísla'
                elif not re.match(r'^[a-zA-ZťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', new_name):
                    msg = 'Meno nemôže obsahovať špeciálne znaky !'
                elif not re.match(r'^[a-zA-ZťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', new_surname):
                    msg = 'Priezvisko nemôže obsahovať špeciálne znaky !'
                elif not new_username or not new_password or not new_email:
                    msg = 'Prosím vyplňte všetky políčka !'
                else:
                    cursor.execute(
                        'UPDATE user SET username= %s ,password= %s,email= %s,name= %s,surname = %s WHERE username=%s', (new_username, secure_password, new_email, new_name, new_surname, username))
                    mysql.connection.commit()
                    edit_succes = True

                if edit_succes:
                    cursor.execute(
                        'SELECT * FROM user where username = %s ', (new_username,))
                    user = cursor.fetchone()
                    msg = 'Zmena údajov uživateľa '+username + ' prebehla úspešne'
                else:
                    cursor.execute(
                        'SELECT * FROM user where username = %s ', (username,))
                    user = cursor.fetchone()

                cursor.close()
                pw = user['password']
                key = 'h8a1dda1xqDas7h4'
                iv = 'IssProjektivpass'
                aes = AES.new(key, AES.MODE_CFB, iv)
                secure_password = aes.decrypt(pw)
                secure_password = secure_password.decode('utf-8')
                user['password'] = secure_password
                return render_template('edit_selected_user.html', account=user, msg=msg)
            else:
                cursor.execute(
                    'SELECT * FROM user where username = %s ', (username,))
                user = cursor.fetchone()
                msg = 'Neboli vyplnené všetky potrebné údaje'
                cursor.close()
                return render_template('edit_selected_user.html', account=user, msg=msg)
        else:
            redirect(url_for('home'))
    else:
        return redirect(url_for('login'))


@app.route('/delete_selected_user/<username>', methods=['GET', 'POST'])
def delete_selected_user(username):
    if 'logged_in' in session:
        if session["role"] == "admin":
            cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
            cursor.execute(
                'DELETE FROM user WHERE username = %s ', (username,))
            mysql.connection.commit()
            cursor.close()
            return redirect(url_for('edit_users'))
        else:
            return redirect(url_for('home'))
    else:
        return redirect(url_for('login'))


@app.route('/select_room_for_presentation/<id_presentation>', methods=['GET', 'POST'])
def select_room_for_presentation(id_presentation):

    return redirect(url_for('home'))


@app.route('/create_room/<id_conference>', methods=['GET', 'POST'])
def create_room(id_conference):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT user_fk FROM conference WHERE id_conference = %s', (id_conference,))
    session_chceck = cursor.fetchone()

    if 'logged_in' in session:
        if session_chceck['user_fk'] == session["id"]:
            msg = ''
            button_message = 'Vytvoriť miestnosť'
            cursor.close()
            return render_template('create_room.html', conference=id_conference, msg=msg, button_message=button_message)
        else:
            cursor.close()
            return redirect(url_for('home'))
    else:
        cursor.close()
        return redirect(url_for('login'))


@app.route('/create_room_process/<id_conference>', methods=['GET', 'POST'])
def create_room_process(id_conference):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT user_fk FROM conference WHERE id_conference = %s', (id_conference,))
    session_chceck = cursor.fetchone()
    if 'logged_in' in session:
        if session_chceck['user_fk'] == session["id"]:

            if request.method == 'POST' and 'nazov_miestnosti' in request.form and 'kapacita_miestnosti' in request.form:
                name = request.form['nazov_miestnosti']
                capacity = request.form['kapacita_miestnosti']

                cursor.execute(
                    'SELECT name FROM room WHERE conference_fk = %s and name = %s', (id_conference, name))
                room_check = cursor.fetchone()
                if room_check:
                    msg = 'Miestnosť s daným názvom už v konferencii existuje'
                    button_message = 'Vytvoriť miestnosť'
                    cursor.close()
                    return render_template('create_room.html', conference=id_conference, msg=msg, button_message=button_message)
                else:
                    cursor.execute(
                        'INSERT INTO  room VALUES(NULL,%s,%s,NULL,%s)', (capacity, name, id_conference))
                    mysql.connection.commit()
                    msg = ''
                    button_message = 'Vytvorit ďalšiu miestnosť'
                    cursor.close()
                    return render_template('create_room.html', conference=id_conference, msg=msg, button_message=button_message)
        else:
            cursor.close()
            redirect(url_for('home'))
    else:
        cursor.close()
        return redirect(url_for('login'))


@app.route('/edit_or_delete_room/<id_conference>')
def edit_or_delete_room(id_conference):
    if 'logged_in' in session:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'SELECT * FROM conference where id_conference = %s ', (id_conference,))
        conference_item = cursor.fetchone()
        if session['id'] == conference_item['user_fk']:
            cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
            cursor.execute(
                'SELECT name FROM room where conference_fk = %s ', (id_conference,))
            rooms = cursor.fetchall()
            cursor.close()
            return render_template('edit_rooms.html', conference=id_conference, find=False, rooms=rooms)
        else:
            cursor.close()
            return redirect(url_for('conference', id_conference=id_conference))
    else:
        return redirect(url_for('login'))


@app.route('/edit_rooms_search/<id_conference>', methods=['GET', 'POST'])
def edit_rooms_search(id_conference):
    msg = ''
    if 'logged_in' in session:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'SELECT * FROM conference where id_conference = %s ', (id_conference,))
        conference_item = cursor.fetchone()
        if session['id'] == conference_item['user_fk']:
            cursor.execute(
                'SELECT name FROM room where conference_fk = %s ', (id_conference,))
            rooms = cursor.fetchall()
            if request.method == "POST" and 'find_room' in request.form:
                room_name = request.form['find_room']
                cursor.execute(
                    'SELECT * FROM room WHERE name = %s and conference_fk = %s', (room_name, id_conference,))
                result = cursor.fetchone()
                if result is not None:
                    cursor.close()
                    return render_template('edit_rooms.html', rooms=result, msg=msg, find=True, conference=id_conference)
                else:
                    cursor.close()
                    msg = 'Miestnosť s daným menom neexistuje'
                    return render_template('edit_rooms.html', rooms=rooms, msg=msg, find=False, conference=id_conference)
            else:
                cursor.close()
                return render_template('edit_rooms.html', rooms=rooms, msg=msg, find=False, conference=id_conference)
        else:
            cursor.close()
            return redirect(url_for('conference', id_conference=id_conference))
    else:
        return redirect(url_for('login'))


@app.route('/edit_seleted_room/<id_conference>/<room>', methods=['GET', 'POST'])
def edit_selected_room(id_conference, room):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT user_fk FROM conference WHERE id_conference = %s', (id_conference,))
    session_chceck = cursor.fetchone()
    if 'logged_in' in session:
        if session_chceck['user_fk'] == session["id"]:
            if request.method == "POST" and 'new_name' in request.form and 'new_capacity' in request.form:
                # return redirect(url_for('login'))
                new_name = request.form['new_name']
                new_capacity = request.form['new_capacity']
                if room != new_name:
                    cursor.execute(
                        'SELECT * FROM room WHERE name=%s and conference_fk = %s', (new_name, id_conference))
                    exist_test = cursor.fetchall()
                    if exist_test:
                        msg = 'Úprava nepebehla úspešne, miestnosť s daným názvom už existuje v tejto konferencii'
                        cursor.close()
                        return render_template('edit_seleted_room.html', id_conference=id_conference, room=room, msg=msg)
                    else:
                        cursor.execute('UPDATE room SET name = %s , capacity = %s WHERE name = %s and conference_fk = %s', (
                            new_name, new_capacity, room, id_conference,))
                        mysql.connection.commit()
                        msg = 'Úprava prebehla úspešne'
                        cursor.close()
                        return render_template('edit_seleted_room.html', id_conference=id_conference, room=new_name, msg=msg)
                else:
                    cursor.execute('UPDATE room SET capacity = %s WHERE name = %s and conference_fk = %s', (
                        new_capacity, room, id_conference))
                    mysql.connection.commit()
                    msg = 'Uprava prebehla úspešne'
                    cursor.close()
                    return render_template('edit_seleted_room.html', id_conference=id_conference, room=room, msg=msg)
            else:
                cursor.close()
                return render_template('edit_seleted_room.html', id_conference=id_conference, room=room)
        else:
            cursor.close()
            redirect(url_for('home'))
    else:
        cursor.close()
        return redirect(url_for('login'))


@app.route('/delete_slected_room/<id_conference>/<room>')
def delete_selected_room(id_conference, room):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT user_fk FROM conference WHERE id_conference = %s', (id_conference,))
    session_chceck = cursor.fetchone()
    if 'logged_in' in session:
        if session_chceck['user_fk'] == session["id"]:
            cursor.execute(
                'DELETE FROM room WHERE  conference_fk = %s and name = %s ', (id_conference, room))
            mysql.connection.commit()
            msg = 'Odstránenie miestnosti prebehlo úspešne'
            cursor.close()
            return redirect(url_for('edit_or_delete_room', id_conference=id_conference))
        else:
            cursor.close()
            return redirect(url_for('home'))
    else:
        cursor.close()
        return redirect(url_for('login'))


@ app.route('/conference/<id_conference>')
def conference(id_conference):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT * FROM conference WHERE id_conference = %s', (id_conference,))
    conference_item = cursor.fetchone()
    cursor.execute(
        'SELECT * FROM presentation,conference WHERE conference.id_conference = presentation.conference_fk AND conference.id_conference = %s AND status ="confirmed"', (id_conference,))
    presentations = cursor.fetchall()

    cursor.execute('SELECT id_ticket,Count(id_ticket) as num_of_tickets FROM ticket WHERE conference_fk = %s GROUP BY %s ',
                   (id_conference, id_conference,))
    number_of_tickets = cursor.fetchone()
    cursor.close()
    return render_template('conference.html', conference=conference_item, presentations=presentations, noft=number_of_tickets)


@ app.route('/create_conference', methods=['GET', 'POST'])
def create_conference():
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    msg = ''
    if request.method == 'POST' and 'name' in request.form and 'field_of_interest' in request.form and 'place' in request.form and 'date_of_start' in request.form and 'date_of_end' in request.form and 'capacity' in request.form:
        name = request.form['name']
        field_of_interest = request.form['field_of_interest']
        place = request.form['place']
        price = request.form['price']
        date_of_start = request.form['date_of_start']
        date_of_end = request.form['date_of_end']
        capacity = request.form['capacity']
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM conference where field_of_interest = %s and name = %s and place = %s and price = %s and capacity = %s and user_fk = %s and date_of_start = %s and date_of_end =%s',
                       (field_of_interest, name, place, price, capacity, session['id'], date_of_start, date_of_end,))
        exist_test = cursor.fetchall()
        if exist_test:
            msg = "Rovnaká konferencia už existuje ! Skontrolujte správnosť zadaných parametrov"
            cursor.close()
            render_template('create_conference.html', msg=msg)
        else:
            time_test_1 = datetime.strptime(date_of_start, '%Y-%m-%dT%H:%M')
            time_test_2 = datetime.strptime(date_of_end, '%Y-%m-%dT%H:%M')

            if time_test_2 <= time_test_1:
                msg = 'Neplatné hodnoty začiatku a konca konferencie'
            elif not re.match(r'^[a-zA-Z0-9ťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', name):
                msg = 'Názov konferencie nemôže obsahovať špeciálne znaky !'
            elif not re.match(r'^[a-zA-Z0-9ťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', place):
                msg = 'Miesto konania konferencie nemôže obsahovať špeciálne znaky !'
            else:
                cursor.execute(
                    'INSERT INTO conference VALUES (NULL,%s,%s,%s,%s,%s,%s,%s,%s)', (field_of_interest, name, place, price, date_of_start, date_of_end, capacity, session['id'],))
                mysql.connection.commit()
                cursor.execute('SELECT * FROM conference where field_of_interest = %s and name = %s and place = %s and price = %s and capacity = %s and user_fk = %s and date_of_start = %s and date_of_end =%s',
                               (field_of_interest, name, place, price, capacity, session['id'], date_of_start, date_of_end,))
                conference_db = cursor.fetchone()
                conference_db_id = conference_db['id_conference']
                msg = 'Konferencia bola vytvorená !'
                button_message = 'Vytvoriť miestnosť'
                cursor.close()
                return render_template('create_room.html', conference=conference_db_id, msg=msg, button_message=button_message)
    elif request.method == 'POST':
        msg = 'Prosím vyplňte všetky políčka !'

    return render_template('create_conference.html', msg=msg)


@ app.route('/buy_ticket/<id_conference>', methods=['GET', 'POST'])
def buy_ticket(id_conference):
    quantity = 0
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    if request.method == 'POST' and 'quantity' in request.form:
        quantity = int(request.form['quantity'])
        cursor.execute('SELECT id_ticket,Count(id_ticket) as num_of_tickets FROM ticket WHERE conference_fk = %s GROUP BY %s ',
                       (id_conference, id_conference,))
        number_of_tickets = cursor.fetchone()

        if number_of_tickets:
            check_sum = quantity + int(number_of_tickets['num_of_tickets'])
        else:
            check_sum = quantity + 0

        cursor.execute(
            'SELECT capacity FROM conference where id_conference = %s', (id_conference,))
        capacity_of_conference = cursor.fetchone()

        if check_sum > int(capacity_of_conference['capacity']):
            cursor.close()
            return render_template('bought_err.html', conference=id_conference)

        class form_tickets(FlaskForm):
            pass
        form_tickets.tickets = FieldList(
            FormField(form_ticket), min_entries=quantity)
        form_tickets.submit = SubmitField("Objednať s povinnosťou platby")
        form = form_tickets()
        cursor.close()
        return render_template('bought.html', form=form, id_conference=id_conference)
    else:
        b = request.form
        names = {x: b[x] for x in b if "tickets-" in x}
        values = list(names.values())
        index = 0

        cursor.execute('SELECT id_ticket,Count(id_ticket) as num_of_tickets FROM ticket WHERE conference_fk = %s GROUP BY %s ',
                       (id_conference, id_conference,))
        number_of_tickets = cursor.fetchone()
        if number_of_tickets:
            check_sum = len(values)/2 + \
                int(number_of_tickets['num_of_tickets'])
        else:
            check_sum = len(values)/2 + 0

        cursor.execute(
            'SELECT capacity FROM conference where id_conference = %s', (id_conference,))
        capacity_of_conference = cursor.fetchone()
        cursor.execute(
            'SELECT * FROM conference where id_conference = %s', (id_conference,))
        conference = cursor.fetchone()
        if int(check_sum) > int(capacity_of_conference['capacity']):
            cursor.close()
            return render_template('bought_err.html', conference=id_conference)
        # values[i] is name and value[i+1] is surname of the person added
        for i in range(0, len(values), 2):
            if 'logged_in' in session:
                if(conference['user_fk'] == session['id']):
                    cursor.execute(
                        'INSERT INTO ticket VALUES (NULL,%s,%s,"confirmed",%s,%s)', (id_conference,session['id'],values[i], values[i+1],))
                else:
                    cursor.execute(
                        'INSERT INTO ticket VALUES (NULL,%s,%s,"waiting_for_confirmation",%s,%s)', (id_conference, session['id'], values[i], values[i+1],))
            else:
                cursor.execute(
                    'INSERT INTO ticket VALUES (NULL,%s,NULL,"waiting_for_confirmation",%s,%s)', (id_conference, values[i], values[i+1],))
        mysql.connection.commit()
        cursor.close()
        return render_template('bought.html', names=values, id_conference=id_conference, len=len(values))


@ app.route('/create_presentation/<id_conference>', methods=['GET', 'POST'])
def create_presentation(id_conference):
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    msg = ''
    lecturer = ''
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT * FROM conference WHERE id_conference = %s', (id_conference,))
    conference_item = cursor.fetchone()
    if("name" in session):
        if(session['name'] is not None):  # creator has to have his name saved
            lecturer = session['name'] + " " + session['surname']
    else:
        cursor.close()
        return render_template('create_presentation.html', msg="Pre zapísanie prezentácie musíte mať vyplnené vaše meno a priezvisko.", creator_id=conference_item['user_fk'], id_conference=id_conference)
    if request.method == 'POST' and 'name' in request.form and 'info' in request.form and 'tags' in request.form:
        name = request.form['name']
        info = request.form['info']
        tags = request.form['tags']
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        if not re.match(r'^[a-zA-Z0-9ťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', name):
            msg = 'Názov prezentácie nemôže obsahovať špeciálne znaky !'
        else:
            if(conference_item['user_fk'] == session['id']):
                cursor.execute(
                    'INSERT INTO presentation VALUES (NULL,%s,%s,%s,%s,NULL,NULL,"confirmed",%s,NULL,%s)', (name, info, lecturer, tags, session['id'], id_conference,))
                mysql.connection.commit()
                msg = 'Prezentácia bola vytvorená !'
                cursor.close()
                return render_template('create_presentation.html', msg=msg, creator_id=conference_item['user_fk'], id_conference=id_conference)
            else:
                cursor.execute(
                    'INSERT INTO presentation VALUES (NULL,%s,%s,%s,%s,NULL,NULL,"waiting_for_confirmation",%s,NULL,%s)', (name, info, lecturer, tags, session['id'], id_conference,))
                mysql.connection.commit()
                cursor.close()
                return render_template('create_presentation.html', msg="Návrh na prezentáciu bol odoslaný správcovi na schválenie", creator_id=conference_item['user_fk'], id_conference=id_conference)
    elif request.method == 'POST':
        msg = 'Prosím vyplňte všetky políčka !'
        cursor.close()
        return render_template('create_presentation.html', msg=msg, creator_id=conference_item['user_fk'], id_conference=id_conference)
    else:
        cursor.close()
        return render_template('create_presentation.html', msg=msg, creator_id=conference_item['user_fk'], id_conference=id_conference)


@ app.route('/edit_conference/<id_conference>/<edit>', methods=['GET', 'POST'])
def edit_conference(id_conference, edit):
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    msg = ''
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('SELECT * FROM conference WHERE id_conference = %s',
                   (id_conference,))
    conference = cursor.fetchone()
    if request.method == 'POST' and 'name' in request.form and 'field_of_interest' in request.form and 'place' in request.form and 'price' in request.form and 'date_of_start' in request.form and 'date_of_end' in request.form and 'capacity' in request.form:
        name = request.form['name']
        field_of_interest = request.form['field_of_interest']
        place = request.form['place']
        price = request.form['price']
        date_of_start = request.form['date_of_start']
        date_of_end = request.form['date_of_end']
        capacity = request.form['capacity']

        time_test_1 = datetime.strptime(date_of_start, '%Y-%m-%dT%H:%M')
        time_test_2 = datetime.strptime(date_of_end, '%Y-%m-%dT%H:%M')
        if time_test_2 <= time_test_1:
            msg = 'Neplatné hodnoty začiatku a konca konferencie'
            cursor.close()
            return render_template('conference.html', id_conference=conference['id_conference'], conference=conference, edit='True', msg=msg)
        elif not re.match(r'^[a-zA-Z0-9ťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', name):
            msg = 'Názov konferencie nemôže obsahovať špeciálne znaky !'
            return render_template('conference.html', id_conference=conference['id_conference'], conference=conference, edit='True', msg=msg)
        elif not re.match(r'^[a-zA-Z0-9ťŤľščĽŠČŽÝÁÍÉžýáíéäôľàáâäãåąčćęèéêëėįìíîïłńòóôöõøùúûüųūÿýżźñçčšžÀÁÂÄÃÅĄĆČĖĘÈÉÊËÌÍÎÏĮŁŃÒÓÔÖÕØÙÚÛÜŲŪŸÝŻŹÑßÇŒÆČŠŽ∂ðŘř,. ]+$', place):
            msg = 'Miesto konania konferencie nemôže obsahovať špeciálne znaky !'
            return render_template('conference.html', id_conference=conference['id_conference'], conference=conference, edit='True', msg=msg)

        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'UPDATE conference SET name= %s ,field_of_interest= %s,place= %s,price= %s,date_of_start = %s,date_of_end = %s,capacity = %s WHERE id_conference=%s', (name, field_of_interest, place, price, date_of_start, date_of_end, capacity, id_conference,))
        mysql.connection.commit()
        if(edit == 'False'):
            cursor.close()
            return redirect(url_for('conference', id_conference=id_conference))
    elif request.method == 'POST':
        msg = 'Prosím vyplňte všetky políčka !'
    cursor.close()
    return render_template('conference.html', id_conference=conference['id_conference'], conference=conference, edit=edit)


@ app.route('/my_tickets')
def my_tickets():
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('SELECT * FROM ticket WHERE user_fk = %s',
                   (session['id'],))
    tickets = cursor.fetchall()
    cursor.execute(
        'SELECT name,id_conference FROM conference')
    conferences = cursor.fetchall()
    cursor.close()
    return render_template('my_tickets.html', tickets=tickets, conferences=conferences)


@ app.route('/tickets/<id_conference>/<id_ticket>')
def tickets(id_conference, id_ticket):
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    if id_ticket == "all":
        cursor.execute(
            'UPDATE ticket SET status="confirmed" WHERE status="waiting_for_confirmation"')
        mysql.connection.commit()
        cursor.close()
        return redirect(url_for('conference', id_conference=id_conference))
    elif id_ticket == "show":
        cursor.execute(
            'SELECT * FROM ticket WHERE conference_fk = %s and status="waiting_for_confirmation"', (id_conference,))
        tickets = cursor.fetchall()
        cursor.close()
        return render_template('tickets.html', tickets=tickets, id_conference=id_conference)
    else:
        cursor.execute(
            'UPDATE ticket SET status= "confirmed" WHERE id_ticket=%s', (id_ticket,))
        mysql.connection.commit()
        cursor.execute(
            'SELECT * FROM ticket WHERE conference_fk = %s', (id_conference,))
        tickets = cursor.fetchall()
        cursor.close()
        return render_template('tickets.html', tickets=tickets, id_conference=id_conference)


@ app.route('/presentation/<id_conference>/<id_presentation>')
def presentation(id_conference, id_presentation):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT * FROM conference WHERE id_conference = %s', (id_conference,))
    conference_item = cursor.fetchone()
    cursor.execute(
        'SELECT * FROM presentation WHERE id_prezentation = %s', (id_presentation,))
    presentation = cursor.fetchone()
    cursor.close()
    return render_template('presentation.html', conference=conference_item, presentation=presentation)


@ app.route('/presentation/<id_conference>/<id_presentation>/<edit>', methods=['GET', 'POST'])
def edit_presentation(id_conference, id_presentation, edit):
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    msg = ''
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('SELECT * FROM conference WHERE id_conference = %s',
                   (id_conference,))
    conference_item = cursor.fetchone()
    cursor.execute('SELECT * FROM presentation WHERE id_prezentation = %s',
                   (id_presentation,))
    presentation = cursor.fetchone()
    if request.method == 'POST' and 'name' in request.form and 'info' in request.form and 'lecturer' in request.form and 'tags' in request.form and 'date_of_start' in request.form and 'date_of_end' in request.form:
        name = request.form['name']
        info = request.form['info']
        lecturer = request.form['lecturer']
        tags = request.form['tags']
        date_of_start = request.form['date_of_start']
        date_of_end = request.form['date_of_end']
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'UPDATE presentation SET name= %s ,info= %s,lecturer= %s,tags= %s,date_of_start = %s,date_of_end = %s ,status = %s WHERE id_prezentation=%s', (name, info, lecturer, tags, date_of_start, date_of_end, 'GG', id_presentation,))
        mysql.connection.commit()
        if(edit == 'False'):
            cursor.close()
            return redirect(url_for('presentation', id_conference=id_conference, id_presentation=id_presentation))
    elif request.method == 'POST':
        msg = 'Prosím vyplňte všetky políčka !'
    cursor.close()
    return render_template('presentation.html', conference=conference_item, presentation=presentation, edit=edit)


@app.route('/show_timeshedule/<id_conference>')
def show_timeshedule(id_conference):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT * FROM presentation,conference WHERE conference.id_conference = presentation.conference_fk AND conference.id_conference = %s ORDER BY presentation.date_of_start', (id_conference,))
    presentations = cursor.fetchall()
    cursor.execute(
        'SELECT * from room where conference_fk = %s', (id_conference,))
    rooms_conf = cursor.fetchall()

    timeshedule_test = False
    for presentation in presentations:
        if presentation['room_fk'] is not None:
            timeshedule_test = True

    if timeshedule_test:
        cursor.close()
        return render_template('show_timetable.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference)
    else:
        msg = "Správca konferencie nevytvoril časový rozvrh"
        cursor.close()
        return render_template('show_timetable.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg)


@app.route('/show_my_presentation_timeshedule/<id_conference>')
def show_my_presentation_timeshedule(id_conference):
    if 'logged_in' in session:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        lecturer = session['name'] + " " + session['surname']
        cursor.execute(
            'SELECT * FROM presentation,conference WHERE conference.id_conference = presentation.conference_fk AND presentation.lecturer =%s AND conference.id_conference = %s ORDER BY presentation.date_of_start', (lecturer, id_conference,))
        presentations = cursor.fetchall()
        cursor.execute(
            'SELECT * from room where conference_fk = %s ', (id_conference,))
        rooms_conf = cursor.fetchall()
        timeshedule_test = False
        rooms = []
        for presentation in presentations:
            id_of_room = presentation['room_fk']
            rooms.append(id_of_room)
            if presentation['room_fk'] is not None:
                timeshedule_test = True

        rooms = list(dict.fromkeys(rooms))

        if timeshedule_test:
            cursor.close()
            return render_template('show_lecturer_timetable.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, active_rooms=rooms)
        else:
            msg = "Správca konferencie nevytvoril časový rozvrh"
            cursor.close()
            return render_template('show_lecturer_timetable.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg)
    else:
        return redirect(url_for('login'))


@app.route('/create_timeshedule/<id_conference>/<id_presentation>', methods=['GET', 'POST'])
def create_timeshedule(id_conference, id_presentation):
    if 'logged_in' in session:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'SELECT * FROM conference where id_conference = %s ', (id_conference,))
        conference_item = cursor.fetchone()
        if session['id'] == conference_item['user_fk']:
            cursor.execute(
                'SELECT * FROM presentation,conference WHERE conference.id_conference = presentation.conference_fk AND conference.id_conference = %s ORDER BY presentation.date_of_start', (id_conference,))
            presentations = cursor.fetchall()
            cursor.execute(
                'SELECT * from room where conference_fk = %s', (id_conference,))
            rooms_conf = cursor.fetchall()

            conference_time_test_1 = datetime.strptime(
                str(conference_item['date_of_start']), '%Y-%m-%d %H:%M:%S')
            conference_time_test_2 = datetime.strptime(
                str(conference_item['date_of_end']), '%Y-%m-%d %H:%M:%S')
            if request.method == "POST" and 'date_of_start' in request.form and 'date_of_end' in request.form and 'rooms' in request.form:
                date_of_start = request.form['date_of_start']
                date_of_end = request.form['date_of_end']
                room = request.form['rooms']
                time_test_1 = datetime.strptime(
                    date_of_start, '%Y-%m-%dT%H:%M')  # dostanem z html
                time_test_2 = datetime.strptime(
                    date_of_end, '%Y-%m-%dT%H:%M')  # dostanem z html
                if time_test_2 <= time_test_1:
                    msg = 'Neplatné hodnoty začiatku a konca prezentácie'
                    cursor.close()
                    return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg, conference_start=conference_time_test_1, conference_end=conference_time_test_2)

                if conference_time_test_1 <= time_test_1 and conference_time_test_2 >= time_test_2:
                    cursor.execute(
                        'SELECT * from room where name = %s and conference_fk = %s', (room, id_conference,))
                    room_id = cursor.fetchone()
                    cursor.execute(
                        'SELECT * FROM presentation where conference_fk=%s and room_fk=%s ', (id_conference, room_id['id_room'],))
                    help_presentations = cursor.fetchall()
                    if help_presentations:
                        for presentation in help_presentations:
                            start = presentation.get('date_of_start')
                            end = presentation.get('date_of_end')
                            start_test = datetime.strptime(
                                str(start), '%Y-%m-%d %H:%M:%S')
                            end_test = datetime.strptime(
                                str(end), '%Y-%m-%d %H:%M:%S')
                            if time_test_1 >= start_test and time_test_2 <= end_test:
                                msg = 'Čas prezentácie sa prelína s prezentáciou: ' + \
                                    str(presentation.get('name'))
                                cursor.close()
                                return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg, conference_start=conference_time_test_1, conference_end=conference_time_test_2)
                            elif time_test_1 < start_test and time_test_2 > end_test:
                                msg = 'Čas prezentácie sa prelína s prezentáciou: ' + \
                                    str(presentation.get('name'))
                                cursor.close()
                                return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg, conference_start=conference_time_test_1, conference_end=conference_time_test_2)
                            elif time_test_1 < start_test and time_test_2 <= end_test and time_test_2 > start_test:
                                msg = 'Čas prezentácie sa prelína s prezentáciou: ' + \
                                    str(presentation.get('name'))
                                cursor.close()
                                return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg, conference_start=conference_time_test_1, conference_end=conference_time_test_2)
                            elif time_test_1 >= start_test and time_test_1 < end_test and time_test_2 > end_test:
                                msg = 'Čas prezentácie sa prelína s prezentáciou: ' + \
                                    str(presentation.get('name'))
                                cursor.close()
                                return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg, conference_start=conference_time_test_1, conference_end=conference_time_test_2)
                            else:
                                cursor.execute('UPDATE presentation SET date_of_start =%s, date_of_end =%s ,room_fk=%s where id_prezentation=%s', (
                                    date_of_start, date_of_end, room_id['id_room'], id_presentation,))
                                mysql.connection.commit()
                                cursor.close()
                                return redirect(url_for('create_timeshedule', id_conference=id_conference, id_presentation='-1'))
                    else:
                        cursor.execute('UPDATE presentation SET date_of_start =%s, date_of_end =%s ,room_fk=%s where id_prezentation=%s', (
                            date_of_start, date_of_end, room_id['id_room'], id_presentation,))
                        mysql.connection.commit()
                        cursor.close()
                        return redirect(url_for('create_timeshedule', id_conference=id_conference, id_presentation='-1'))
                else:
                    msg = 'Zvolený čas prednášky je mimo času konania konferencie'
                    cursor.close()
                    return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, msg=msg, conference_start=conference_time_test_1, conference_end=conference_time_test_2)
            else:
                cursor.close()
                return render_template('create_timeshedule.html', presentations=presentations, conf_rooms=rooms_conf, id_conference=id_conference, conference_start=conference_time_test_1, conference_end=conference_time_test_2)
        else:
            cursor.close()
            return redirect(url_for('conference', id_conference=id_conference))
    else:
        return redirect(url_for('login'))


@app.route('/room/<id_conference>/<room_name>',)
def room(id_conference, room_name):
    return redirect(url_for('home'))


@app.route('/show_rooms/<id_conference>')
def show_rooms(id_conference):
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(
        'SELECT * FROM room,conference WHERE conference.id_conference = room.conference_fk AND conference.id_conference = %s ', (id_conference,))
    rooms = cursor.fetchall()
    cursor.close()
    return render_template('show_rooms.html', id_conference=id_conference, conf_rooms=rooms)


@ app.route('/confirm_presentations/<id_conference>/<id_presentation>')
def confirm_presentations(id_conference, id_presentation):
    if not 'logged_in' in session:
        return redirect(url_for('login'))
    cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
    if id_presentation == "all":
        cursor.execute(
            'UPDATE presentation SET status="confirmed" WHERE status="waiting_for_confirmation"')
        mysql.connection.commit()
        cursor.close()
        return redirect(url_for('conference', id_conference=id_conference))
    elif id_presentation == "show":
        cursor.execute(
            'SELECT * FROM presentation WHERE conference_fk = %s and status="waiting_for_confirmation"', (id_conference,))
        presentations = cursor.fetchall()
        cursor.close()
        return render_template('confirm_presentations.html', presentations=presentations, id_conference=id_conference)
    else:
        cursor.execute(
            'UPDATE presentation SET status= "confirmed" WHERE id_prezentation=%s', (id_presentation,))
        mysql.connection.commit()
        cursor.execute(
            'SELECT * FROM presentation WHERE conference_fk = %s', (id_conference,))
        presentations = cursor.fetchall()
        cursor.close()
        return render_template('confirm_presentations.html', presentations=presentations, id_conference=id_conference)


if __name__ == "__main__":
    app.run()
