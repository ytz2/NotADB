import xml.etree.ElementTree as ET
import sys
import pprint
import json

def todict(obj, classkey=None):
    if isinstance(obj, dict):
        data = {}
        for (k, v) in obj.items():
            data[k] = todict(v, classkey)
        return data
    elif hasattr(obj, "_ast"):
        return todict(obj._ast())
    elif hasattr(obj, "__iter__") and not isinstance(obj, str):
        return [todict(v, classkey) for v in obj]
    elif hasattr(obj, "__dict__"):
        data = dict([(key, todict(value, classkey)) 
            for key, value in obj.__dict__.items() 
            if not callable(value) and not key.startswith('_')])
        if classkey is not None and hasattr(obj, "__class__"):
            data[classkey] = obj.__class__.__name__
        return data
    else:
        return obj
def uniq(l):
    m = {}
    for each in l:
        if each.id in m:
            continue
        m[each.id] = each
    return list(m.values())

class Message:
    def __init__(self):
        self.id = ""
        self.name = ""
        self.fields = []
        self.requiredFields = []

class Field:
    def __init__(self):
        self.id = 0
        self.name = ""
        self.type = ""
        self.cpptype = ""
        self.isEnum = False

class Enum:
    def __init__(self):
        self.id = 0
        self.name = ""
        self.values = []

class EnumValue:
    def __init__(self):
        self.name = ""
        self.value = ""

class XMLProtocol:
    def toJson(self):
        d = {
            "version": self.version,
            "messages": todict(self.messages),
            "fields": todict(self.fields),
            "enums": todict(self.enums),
            "groups": todict(self.groups)
        }
        return json.dumps(d)

    def __init__(self, fln):
        self.version = None
        self.messages = []
        self.groups = []
        self.fields = []
        self.enums = []
        self.root =  ET.parse(fln).getroot()
        self.cppType = {
            #fix type: cpp media, 
            'INT': 'int',
            'DAYOFMONTH': 'int', # 1-31
            'QTY':'double',
            'PRICE': 'double',
            'LENGTH': 'int',
            'PRICEOFFSET': 'double',
            'FLOAT': 'double',
            'CHAR': 'char',
            'DATA': 'string',
            'AMT': 'double',
            'BOOLEAN': 'bool', #Y, N
            'STRING': 'string',
            'MULTIPLEVALUESTRING': 'string',
            'CURRENCY': 'string',
            'EXCHANGE': 'string',
            'UTCTIMESTAMP': 'time', # YYYYMMDD-HH:MM:SS.sss
            'UTCTIMEONLY':'time', #HH:MM:SS.sss
            'LOCALMKTDATE': 'time', # YYYYMMDD,
            'UTCDATE': 'time',# YYYYMMDD
            'MONTHYEAR': 'time', #YYYYMM
        }
    
    def parse(self):
        self.version = self.parseVersion(self.root)   
        self.parseFields()  
        self.fields = sorted(self.fields,  key=lambda f: f.id) 
        self.enums = sorted(self.enums, key = lambda e: e.id)
        header = Message()
        header.id = 1001
        header.name = 'header'
        self.parseObject(self.root.find('header'), header, False)
        trailer = Message()
        trailer.id = 1002
        trailer.name = 'trailer'
        self.parseObject(self.root.find('trailer'), trailer, False)
        self.parseMessage()
        self.messages = sorted(self.messages, key = lambda m: m.id)
        self.groups = sorted(uniq(self.groups), key = lambda m: m.id)

    def parseObject(self, xml, obj, isMsg):
        for each in xml:
            tag = each.tag
            req = each.get('required')
            name = each.get('name')
            isRequried = req == 'Y'
            f = self.findField(name)
            obj.fields.append(f.id)
            if isRequried:
                obj.requiredFields.append(f.id)
            if tag == 'group':
                grp = Message()
                grp.name = name
                grp.id = f.id
                self.parseObject(each, grp, False)
        if isMsg:
            self.messages.append(obj)
        else:
            self.groups.append(obj)

    def parseMessage(self):
        messages = self.root.find('messages')
        for msg in messages:
            m = Message()
            m.id = msg.get('msgtype')
            m.name = msg.get('name')
            self.parseObject(msg, m, True)
            #header and trailer
            m.fields.insert(0, 1001)
            m.requiredFields.insert(0, 1001)
            m.fields.append(1002)
            m.requiredFields.append(1002)

    def findField(self, name):
        for each in self.fields:
            if each.name == name:
                return each
        return None

    def parseFields(self):
        fields = self.root.find('fields')
        for field in fields:
            f = Field()
            f.id = int(field.get('number'))
            f.name = field.get('name')
            f.type = field.get('type')
            f.cpptype = self.cppType[f.type]
            enums = field.findall('value')
            if len(enums) == 0 :
                f.isEnum = False
            else:
                enumDef = Enum()
                enumDef.name = f.name
                enumDef.id = f.id
                for enum in enums: 
                    enumField = EnumValue()
                    enumField.value = enum.get('enum')
                    enumField.name = enum.get('description')
                    enumDef.values.append(enumField)
                self.enums.append(enumDef)
            self.fields.append(f)

    def parseVersion(self, child):
        ret = child.attrib['type'] +"."+child.attrib['major']+'.'+child.attrib['minor']
        if child.attrib['servicepack'] != '0':
            ret = ret + '.'+ child.attrib['servicepack'] 
        return ret



fln = sys.argv[1]
protocol = XMLProtocol(fln)
protocol.parse()
jsondata = protocol.toJson()
dataFile = open(fln.split('.')[0]+'.json', "w")
dataFile.write(json.dumps( json.loads(jsondata), indent=4, sort_keys=True))
dataFile.close()