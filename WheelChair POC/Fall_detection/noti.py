from plyer import notification

def send_notification(title, message):
    notification.notify(
        title=title,
        message=message,
        app_name='Sample Notification',  # Specify your app name
        timeout=10,  # Notification will disappear after 10 seconds
    )

# Example usage
if __name__ == "__main__":
    send_notification("Alert!", "Your patient's wheelchair crashed!, Go2 this location ...")
