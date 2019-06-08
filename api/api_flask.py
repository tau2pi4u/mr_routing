from flask import Flask
from flask_restful import Api
from node_end import node_end
from config import NodesFromConfig
import json
import os

with open(os.environ.get('MR_ROUTING_API_CONFIG'), "r") as configJson:
    cfg = json.load(configJson)

app = Flask(__name__)
api = Api(app)

key = {}
db = {
    'address_map' : cfg['address_map'],
    'nodes' : NodesFromConfig(cfg)
}

api.add_resource(node_end, '/api/node/<node_id>', resource_class_args=(db,))