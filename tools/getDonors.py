import json
import re
import requests
import sys

def queryGitHub(q):
    res = requests.post("https://api.github.com/graphql",
        data = json.dumps({"query": q}),
        headers = {
            "Authorization": "token " + sys.argv[1]
        })
    return res.json()["data"]

def getGitHub():
    after = ""
    markdown = ""

    while True:
        data = queryGitHub('''
        query {
          organization(login: "cpeditor") {
            sponsors(first: 100''' + (f', after: "{after}"' if after else "") + ''') {
              pageInfo {
                endCursor
                hasNextPage
              }
              edges {
                node {
                  ... on User {
                    login
                    name
                  }
                  ... on Organization {
                    login
                    name
                  }
                }
              }
            }
          }
        }
        ''')
        sponsors = data["organization"]["sponsors"]
        edges = sponsors["edges"]
        for edge in edges:
            markdown += f'-   [{edge["node"]["name"]}](https://github.com/{edge["node"]["login"]})\n'
        if not sponsors["pageInfo"]["hasNextPage"]:
            break
        after = sponsors["pageInfo"]["endCursor"]

    return markdown

def queryOpenCollective(q):
    res = requests.post("https://opencollective.com/api/graphql/v2",
        data = json.dumps({"query": q}),
        headers = {
            "content-type": "application/json"
        })
    return res.json()["data"]

def getOpenCollective():
    markdown = ""
    data = queryOpenCollective('''
    query {
      collective(slug: "cpeditor") {
        contributors(limit: 1000, roles: [BACKER]) {
          nodes {
            name
            collectiveSlug
          }
        }
      }
    }
    ''')
    nodes = data["collective"]["contributors"]["nodes"]
    for node in nodes:
        markdown += f'-   [{node["name"]}](https://opencollective.com/{node["collectiveSlug"]})\n'
    return markdown

github = getGitHub()
opencollective = getOpenCollective()

with open("DONORS.md", "r") as donors:
    content = donors.read()

newContent = re.sub(r"(?<=<!\-\- START: GitHub Sponsors \-\->)[\s\S]*(?=<!\-\- END: GitHub Sponsors \-\->)", f"\n{github}", content)
newContent = re.sub(r"(?<=<!\-\- START: OpenCollective Contributors \-\->)[\s\S]*(?=<!\-\- END: OpenCollective Contributors \-\->)", f"\n{opencollective}", newContent)

with open("DONORS.md", "w") as donors:
    donors.write(newContent)
