from flask_restful import Resource, reqparse

class new_node_end(Resource):
    def __init__(self, db):
        self.db = db
        self.put_parse = reqparse.RequestParser()
        
    
    def put(self):
        args = self.put_parse.parse_args()

class node_end(Resource):
    def __init__(self, db):
        self.db = db
        self.put_parse = reqparse.RequestParser()
        self.put_parse.add_argument('mac_address', location='form', nullable=True, type=str)
        self.put_parse.add_argument('sender_id', location='form', nullable=True, type=str)
        self.put_parse.add_argument('train_id', location='form', nullable=True, type=str)
    
    def put(self, node_id):
        args = self.put_parse.parse_args()
        if node_id == 'new_node':
            if 'mac_address' not in args.keys():
                return {'error' : f'No mac address given in args'}
            if args['mac_address'] in self.db['address_map'].keys():
                node_id = self.db['address_map'][args['mac_address']]
                return self.db['nodes'][node_id]
            else:
                return {'error' : f'address {args["mac_address"]} is not in the database'}
        elif node_id == 'remove_train':
            return {'error' : 'not implemented'}
        else:
            if 'sender_id' not in args.keys():
                return {'error': f'No sender id given'}
            if 'train_id' not in args.keys():
                return {'error': f'No train id given'}
            src = args['sender_id']
            if node_id not in self.db['nodes'].keys():
                return {'error': f'node {node_id} does not exist'}
            if src not in self.db['nodes'][node_id]['inbound'].keys():
                return {'error': f'node {node_id} does not have source {src}'}
            if self.db['nodes'][node_id]['inbound'][src] != '':
                return {'error': f'Line occupied'}
            for nSrc, nId in self.db['nodes'][src]['inbound'].items():
                if nId == args['train_id']:
                    self.db['nodes'][nSrc]['outbound'][src] = ''
                    self.db['nodes'][src]['inbound'][nSrc] = ''
                    break
            self.db['nodes'][src]['outbound'][node_id] = args['train_id']
            self.db['nodes'][node_id]['inbound'][src] = args['train_id']
            return {}

    def get(self, node_id):
        if node_id in self.db['nodes'].keys():
            return self.db['nodes'][node_id]
        else:
            return {'error': f'node {node_id} does not exist'}

