import joblib
import numpy as np
from pathlib import Path

# 1. Xác định đường dẫn
BASE_DIR = Path(__file__).resolve().parent
RESULT_DIR = BASE_DIR / "results"

# 2. Load các mô hình đã huấn luyện
knn = joblib.load(RESULT_DIR / "knn.pkl")
decision_tree = joblib.load(RESULT_DIR / "decision_tree.pkl")
svm = joblib.load(RESULT_DIR / "svm.pkl")

# 3. Load bộ chuẩn hóa và mã hóa nhãn
scaler = joblib.load(RESULT_DIR / "scaler.pkl")
encoder = joblib.load(RESULT_DIR / "label_encoder.pkl")

# 4. Dữ liệu cảm biến mới
# Bạn có thể thay đổi hai giá trị này
temperature = 10
humidity = 82

new_sensor = np.array([[temperature, humidity]])

# 5. Chuẩn hóa dữ liệu
new_sensor = scaler.transform(new_sensor)

# 6. Dự đoán bằng 3 mô hình
models = {
    "K-NN": knn,
    "Decision Tree": decision_tree,
    "SVM": svm
}

print("\n===== SENSOR PREDICTION =====")
print(f"Temperature: {temperature}")
print(f"Humidity: {humidity}")

for name, model in models.items():
    prediction = model.predict(new_sensor)
    status = encoder.inverse_transform(prediction)[0]

    print(f"{name}: {status}")
