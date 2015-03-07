#!/usr/bin/env python

from wordnik import *
import ConfigParser

config = ConfigParser.ConfigParser()
config.read('config.txt')

apiUrl = 'http://api.wordnik.com/v4'
apiKey = config.get('wordnik', 'api_key')
client = swagger.ApiClient(apiKey, apiUrl)


wordApi = WordApi.WordApi(client)
definitions = wordApi.getDefinitions('badger',
                                     # partOfSpeech='verb',
                                     # sourceDictionaries='wiktionary',
                                     limit=1)
print definitions[0].text
