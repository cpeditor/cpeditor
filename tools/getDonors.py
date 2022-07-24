import json
import re
import requests
import sys

def queryGitHub(q):
    print('GitHub query:')
    print(q)
    res = requests.post("https://api.github.com/graphql",
        data = json.dumps({"query": q}),
        headers = {
            "Authorization": "token " + sys.argv[1]
        })
    print('GitHub response:')
    print(res.text)
    return res.json()["data"]

def getGitHub(donors):
    after = ""

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
            donors[f'https://github.com/{edge["node"]["login"]}'] = edge["node"]["name"]
        if not sponsors["pageInfo"]["hasNextPage"]:
            break
        after = sponsors["pageInfo"]["endCursor"]

def queryOpenCollective(q):
    print('OpenCollective query:')
    print(q)
    res = requests.post("https://opencollective.com/api/graphql/v2",
        data = json.dumps({"query": q}),
        headers = {
            "content-type": "application/json"
        })
    print('OpenCollective response:')
    print(res.text)
    return res.json()["data"]

def getOpenCollective(donors):
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
        if (node["collectiveSlug"] == "github-sponsors"):
            continue
        donors[f'https://opencollective.com/{node["collectiveSlug"]}'] = node["name"]

donors = [{}, {}]

with open("DONORS.md", "r") as donorsFile:
    content = donorsFile.read()

oldMarkdown = [
    re.search(r"(?<=<!\-\- START: GitHub Sponsors \-\->)[\s\S]*(?=<!\-\- END: GitHub Sponsors \-\->)", content),
    re.search(r"(?<=<!\-\- START: OpenCollective Contributors \-\->)[\s\S]*(?=<!\-\- END: OpenCollective Contributors \-\->)", content)
]

for idx, old in enumerate(oldMarkdown):
    if old:
        for line in old.group(0).split('\n'):
            match = re.fullmatch(r"-   \[(.+?)\]\((http.+)\)", line)
            if match:
                donors[idx][match.group(2)] = match.group(1)

getGitHub(donors[0])
getOpenCollective(donors[1])

newMarkdown = ["\n", "\n"]
for i in range(2):
    for url, name in donors[i].items():
        newMarkdown[i] += f'-   [{name}]({url})\n'

newContent = re.sub(r"(?<=<!\-\- START: GitHub Sponsors \-\->)[\s\S]*(?=<!\-\- END: GitHub Sponsors \-\->)", newMarkdown[0], content)
newContent = re.sub(r"(?<=<!\-\- START: OpenCollective Contributors \-\->)[\s\S]*(?=<!\-\- END: OpenCollective Contributors \-\->)", newMarkdown[1], newContent)

with open("DONORS.md", "w") as donors:
    donors.write(newContent)
