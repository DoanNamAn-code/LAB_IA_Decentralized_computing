import joblib
from pathlib import Path
import pandas as pd

# Xác định thư mục results
BASE_DIR = Path(__file__).resolve().parent.parent
RESULT_DIR = BASE_DIR / "results"


class MLPredictor:
    def __init__(self):
        self.knn = joblib.load(RESULT_DIR / "knn.pkl")
        self.decision_tree = joblib.load(
            RESULT_DIR / "decision_tree.pkl"
        )
        self.svm = joblib.load(RESULT_DIR / "svm.pkl")

        self.scaler = joblib.load(RESULT_DIR / "scaler.pkl")
        self.encoder = joblib.load(
            RESULT_DIR / "label_encoder.pkl"
        )

    def predict(self, temperature, humidity):
        # 1. Tạo dữ liệu cảm biến
        sensor_data = pd.DataFrame(
            [[temperature, humidity]],
            columns=["Temperature", "Humidity"]
        )

        # 2. Chuẩn hóa dữ liệu bằng scaler đã huấn luyện
        scaled_data = self.scaler.transform(sensor_data)

        # 3. Dự đoán bằng 3 mô hình
        models = {
            "KNN": self.knn,
            "Decision Tree": self.decision_tree,
            "SVM": self.svm
        }

        results = {}

        for name, model in models.items():
            prediction = model.predict(scaled_data)

            status = self.encoder.inverse_transform(
                prediction
            )[0]

            results[name] = status

        return results
