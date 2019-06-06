def NodesFromConfig(cfg):
    nodes = {node : { \
            "id" : node,
            "inbound" : {},
            "outbound" : {}
            }
        for node in cfg['address_map'].values()
        }
    for connection in cfg['connections']:
        assert(len(connection) == 3)
        src = connection[0]
        dst = connection[1]
        mode = connection[2]
        assert(src in nodes.keys())
        assert(dst in nodes.keys())
        assert(mode in ['both', 'one-way'])
        nodes[src]['outbound'][dst] = ''
        nodes[dst]['inbound'][src] = ''
        if mode == 'both':
            nodes[dst]['outbound'][src] = ''
            nodes[src]['inbound'][dst] = ''
    return nodes