from influxdb import InfluxDBClient, DataFrameClient

URL = "http://147.92.111.86:8086"
HOST = "147.92.111.86"
PORT = "8086"
username = "soteloju"
password = "password"
TOKEN = "MEU_73dDATRXTUs_8GlgZjI6WfQMftKHz7LM8xMnj__rWwokeZqQAHdd9jfD8KzLDEe2O70nWI-OOiHHUWbctA=="
ORG = "4402d84fd4eb6722"

BUCKET = 'SR-20'

# client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
client = InfluxDBClient(host=HOST, port=PORT,  username=username, password=password, database=BUCKET)
# query_api = client.query_api()

dbs = client.get_list_measurements()
print(dbs)

query1 = f'from(bucket:"{BUCKET}")\
|> range(start: -15m)'

query2 = f'from(bucket:"{BUCKET}")\
|> range(start: -15m)\
|> filter(fn: (r) => r["_measurement"] == "processes")\
|> filter(fn: (r) => r["_field"] == "running" or r["_field"] == "paging" or r["_field"] == "sleeping")'

results = client.query_api().query(org=ORG, query=query1)
for table in results:
    for record in table.records:
        print(record)

# records = client.query_api().query_stream(org=ORG, query=query1)
# for record in records:
#     print(record.get_measurement())