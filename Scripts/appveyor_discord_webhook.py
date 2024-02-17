import os
import sys
import json
import urllib.request

BUILD_STATUS = sys.argv[1]

WEBHOOK_URL = os.environ.get("WEBHOOK_URL")

if WEBHOOK_URL is None:
    print("WEBHOOK_URL env var not set")
    sys.exit(0)

BUILD_ID = os.environ.get("APPVEYOR_BUILD_ID")
COMMIT_AUTHOR = os.environ.get("APPVEYOR_REPO_COMMIT_AUTHOR")
COMMIT_MESSAGE = os.environ.get("APPVEYOR_REPO_COMMIT_MESSAGE")
BUILD_VERSION = os.environ.get("APPVEYOR_BUILD_VERSION")
JOB_NAME = os.environ.get("APPVEYOR_JOB_NAME")
REPO_NAME = os.environ.get("APPVEYOR_REPO_NAME")
COMMIT_ID = os.environ.get("APPVEYOR_REPO_COMMIT")
COMMIT_URL = f"https://github.com/{REPO_NAME}/commit/{COMMIT_ID}"

ANNOYING_SLURG_EMOTE = "https://cdn.discordapp.com/emojis/803379764270333953.webp"
SMILE_SCRAB_EMOTE = "https://cdn.discordapp.com/emojis/841720862524309544.webp"

COLOR_RED = 12058624
COLOR_GREEN = 47119

if BUILD_STATUS == "":
    print("unknown build status")
    sys.exit(0)

footer_text = ""
footer_url = ""
embed_color = 1

if BUILD_STATUS == "success":
    footer_text = "Succeeded"
    footer_url = SMILE_SCRAB_EMOTE
    embed_color = COLOR_GREEN
elif BUILD_STATUS == "failure":
    footer_text = "Failed"
    footer_url = ANNOYING_SLURG_EMOTE
    embed_color = COLOR_RED

webhook_message = {
  "content": None,
  "embeds": [
    {
      "title": f"Build {BUILD_ID}",
      "color": embed_color,
      "fields": [
        {
          "name": "Commit",
          "value": f"[{COMMIT_MESSAGE}]({COMMIT_URL})"
        },
        #{
        #  "name": "Duration",
        #  "value": "DURATION_TIME",
        #  "inline": True
        #},
        {
          "name": "Build version",
          "value": BUILD_VERSION,
          "inline": True
        }
      ],
      "author": {
        "name": COMMIT_AUTHOR
      },
      "footer": {
        "text": f"{JOB_NAME} {footer_text}",
        "icon_url": footer_url
      }
    }
  ],
  "attachments": []
}

req = urllib.request.Request(WEBHOOK_URL, json.dumps(webhook_message).encode())
req.add_header("Content-Type", "application/json")
req.add_header("User-Agent", "Mozilla/5.0")
response = urllib.request.urlopen(req)
