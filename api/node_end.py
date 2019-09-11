from flask_restful import Resource, reqparse, request

# For error codes, see error.hpp in the arduino source files
# Will be moved to common place in future

def ValidateArgsForTrainRequest(db, args, node_id):
    if 'sender_id' not in args.keys():
        return {'error': 5}
    if 'train_id' not in args.keys():
        return {'error': 6}
    if node_id not in db['nodes'].keys():
        return {'error': 7}
    if args['sender_id'] not in db['nodes'][node_id]['inbound'].keys():
        return {'error': 8}
    if db['nodes'][node_id]['inbound'][args["sender_id"]] != '':
        return {'error': 1}
    return {}

def ClearLines(db, args, src):
    for nSrc, nId in db['nodes'][src]['inbound'].items():
        if nId == args['train_id']:
            db['nodes'][nSrc]['outbound'][src] = ''
            db['nodes'][src]['inbound'][nSrc] = ''
            break
    return db

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
        args = request.get_json(force=True)
        if node_id == 'new_node':
            if 'mac_address' not in args.keys():
                return {'error' : 2}
            if args['mac_address'] in self.db['address_map'].keys():
                node_id = self.db['address_map'][args['mac_address']]
                return self.db['nodes'][node_id]
            else:
                return {'error' : 7}
        elif node_id == 'remove_train':
            return {'error' : 3}
        else:
            error = ValidateArgsForTrainRequest(self.db, args, node_id)
            if('error' in error.keys()):
                return error
            src = args['sender_id']
            self.db = ClearLines(self.db, args, src)
            self.db['nodes'][src]['outbound'][node_id] = args['train_id']
            self.db['nodes'][node_id]['inbound'][src] = args['train_id']
            return {}

    def get(self, node_id):
        if node_id in self.db['nodes'].keys():
            return self.db['nodes'][node_id]
        else:
            return {}

