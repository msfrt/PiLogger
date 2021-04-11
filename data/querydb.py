from influxdb_client import InfluxDBClient
import pandas as pd

MEASUREMENTS = ['cpu','disk','diskio','kernel','mem','net','processes','swap',
                'system','temp','weather','wireless']

URL = "http://147.92.111.86:8086"
TOKEN = "MEU_73dDATRXTUs_8GlgZjI6WfQMftKHz7LM8xMnj__rWwokeZqQAHdd9jfD8KzLDEe2O70nWI-OOiHHUWbctA=="
ORG = "4402d84fd4eb6722"

BUCKET = 'SR-20'

client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
query_api = client.query_api()

query1 = f'from(bucket:"{BUCKET}")\
|> range(start: -15m)'

query2 = f'from(bucket:"{BUCKET}")\
|> range(start: -15m)\
|> filter(fn: (r) => r["_measurement"] == "processes")'

def df_to_csv(df, output_file):
    try:
        df.to_csv(output_file, index=False)
    except:
        print("An error occurred")
        
def output_csv(values):
    a = [f'r["_measurement"] == "{m}"' for m,v in zip(MEASUREMENTS,values) if v]
    filters = " or ".join(a)
    query = f'from(bucket:"{BUCKET}")\
|> range(start: -1h)\
|> filter(fn: (r) => {filters})'
    print(query)

    data_frame = query_api.query_data_frame(query)
    df_to_csv(data_frame, 'data.csv')
    

# results = client.query_api().query(org=ORG, query=query1)
# for table in results:
#     for record in table.records:
#         print(record)

# records = client.query_api().query_stream(org=ORG, query=query1)
# for record in records:
#     print(record.get_measurement())