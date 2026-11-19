import pandas as pd

# Doc du lieu tu file CSV
data = pd.read_csv("dataset/sensor_fake_data.csv")

# Hien thi toan bo Dataset
print("Dataset:")
print(data)

# Thong tin co ban
print("\nDataset information:")
print(data.info())

# Hien thi 5 dong dau tien
print("\nFirst 5 rows:")
print(data.head())