from flask import Flask, request
import webbrowser
import random
from time import sleep
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL
from plyer import notification
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from geopy.geocoders import Nominatim


app = Flask(__name__)
allSites = [
    "https://github.com/crashPolicyAI/crashPolicyAI"       #add websites
]
devices = AudioUtilities.GetSpeakers()
interface = devices.Activate(
    IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
volume = cast(interface, POINTER(IAudioEndpointVolume))

@app.route("/wheelchair/<status>", methods=["GET"])
def door(status):
    if request.method == "GET":
        print("Inside the crash moment")
        print(status)
        if (status == "OPEN"):
            website = random.choice(allSites)       #grabs website
            sleep(2)            
            # pag.press("volumemute")                 #volume balancer
           
            volume.SetMasterVolumeLevel(-50.0, None) 
            send_notification("Alert!", "Your patient's wheelchair has crashed!, Go2 this location ...")  #This calls the send_noti function
            send_email()

            webbrowser.open_new(website)            #new  tab opened
            return "Message Received"               #return a all good message to protocol -> esp32
        

def send_notification(title, message):
    notification.notify(
        title=title,
        message=message,
        app_name='Alert Notification',  # Specify your app name
        timeout=10,  # Notification will disappear after 10 seconds
    )

def send_email():
    # Initialize Nominatim API
    geolocator = Nominatim(user_agent="Name of the user")

    # Get coordinates for a location (address)
    location = geolocator.geocode("801 Atlantic Dr NW, Atlanta, GA 30332")
    if location:
        print(f"Location Coordinates: Latitude={location.latitude}, Longitude={location.longitude}")
    else:
        print("Location not found.")

    # Set your Gmail credentials
    gmail_user = "crashpolicyai@gmail.com"
    gmail_password = "lwkf qrdr hzja lncf"

    reciever = "aksg.intel@gmail.com"
    # Create the email message
    msg = MIMEMultipart()
    msg['From'] = gmail_user
    msg['To'] = reciever
    msg['Subject'] = "Alert: Robert 'Trey' Quinn"

    # Attach the body of the email
    body = f"Alert, your patient has crashed Latitude={location.latitude}, Longitude={location.longitude}. Condition: cerebrel palsy  ||||||   Blood: O-  |||||||  yoB: 1999  ||||||"
    msg.attach(MIMEText(body, 'plain'))

    # Connect to the Gmail server
    with smtplib.SMTP('smtp.gmail.com', 587) as server:
        server.starttls()
        
        # Log in to your Gmail account
        server.login(gmail_user, gmail_password)
        
        # Send the email
        server.sendmail(gmail_user, reciever, msg.as_string())

    print("Email sent successfully")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=7999)              #runs across your whole local network on port 7999
    





