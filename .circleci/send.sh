#!/bin/bash

case $1 in
  "success" )
    EMBED_COLOR=3066993
    STATUS_MESSAGE="Passed"
    ;;

  "failure" )
    EMBED_COLOR=15158332
    STATUS_MESSAGE="Failed"
    ;;

  * )
    EMBED_COLOR=0
    STATUS_MESSAGE="Status Unknown"
    ;;
esac

shift

if [ $# -lt 1 ]; then
  echo -e "WARNING!!\nYou need to pass the WEBHOOK_URL environment variable as the second argument to this script." && exit
fi

AUTHOR_NAME="$(git log -1 "$CIRCLE_SHA1" --pretty="%aN")"
COMMITTER_NAME="$(git log -1 "$CIRCLE_SHA1" --pretty="%cN")"
COMMIT_SUBJECT="$(git log -1 "$CIRCLE_SHA1" --pretty="%s")"
COMMIT_MESSAGE="$(git log -1 "$CIRCLE_SHA1" --pretty="%b")" | sed -E ':a;N;$!ba;s/\r{0,1}\n/\\n/g'

if [ ${#COMMIT_SUBJECT} -gt 256 ]; then
  COMMIT_SUBJECT="$(echo "$COMMIT_SUBJECT" | cut -c 1-253)"
  COMMIT_SUBJECT+="..."
fi

if [ -n $COMMIT_MESSAGE ] && [ ${#COMMIT_MESSAGE} -gt 1900 ]; then
  COMMIT_MESSAGE="$(echo "$COMMIT_MESSAGE" | cut -c 1-1900)"
  COMMIT_MESSAGE+="..."
fi

if [ "$AUTHOR_NAME" == "$COMMITTER_NAME" ]; then
  CREDITS="$AUTHOR_NAME authored & committed"
else
  CREDITS="$AUTHOR_NAME authored & $COMMITTER_NAME committed"
fi

pwd
ls /home/circleci/project/.circleci

TIMESTAMP=$(date --utc +%FT%TZ)
WEBHOOK_DATA='{
  "username": "",
  "avatar_url": "https://thumbs.gfycat.com/AssuredGiftedBichonfrise-max-1mb.gif",
  "embeds": [ {
    "color": '$EMBED_COLOR',
    "author": {
      "name": "Job #'"$CIRCLE_BUILD_NUM"' '"$STATUS_MESSAGE"' - '"$CIRCLE_PROJECT_REPONAME"'",
      "url": "'"$CIRCLE_BUILD_URL"'",
      "icon_url": "'$AVATAR'"
    },
    "title": "'"$COMMIT_SUBJECT"'",
    "url": "'"$URL"'",
    "description": "'"${COMMIT_MESSAGE//$'\n'/ }"\\n\\n"$CREDITS"'",
    "fields": [
      {
        "name": "Commit",
        "value": "'"[\`${CIRCLE_SHA1:0:7}\`](https://bitbucket.org/xunteam/$CIRCLE_PROJECT_REPONAME/commits/$CIRCLE_SHA1)"'",
        "inline": true
      },
      {
        "name": "Branch",
        "value": "'"[\`$CIRCLE_BRANCH\`](https://bitbucket.org/xunteam/$CIRCLE_PROJECT_REPONAME/src/$CIRCLE_BRANCH)"'",
        "inline": true
      }
    ],
    "timestamp": "'"$TIMESTAMP"'"
  } ]
}'

for ARG in "$@"; do
  echo -e "[Webhook]: Sending webhook to Discord...\\n";

  (curl --fail --progress-bar -A "TravisCI-Webhook" -H Content-Type:application/json -H X-Author:k3rn31p4nic#8383 -d "${WEBHOOK_DATA//	/ }" "$ARG" \
  && echo -e "\\n[Webhook]: Successfully sent the webhook.") || echo -e "\\n[Webhook]: Unable to send webhook."
done
