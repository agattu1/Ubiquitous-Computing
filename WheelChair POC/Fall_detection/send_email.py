import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from geopy.geocoders import Nominatim

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