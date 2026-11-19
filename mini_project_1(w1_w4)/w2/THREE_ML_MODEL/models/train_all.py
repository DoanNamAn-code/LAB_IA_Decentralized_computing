import pandas as pd

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.metrics import accuracy_score

from knn import train_knn
from decision_tree import train_decision_tree
from svm import train_svm


# 1. Load data
data = pd.read_csv("dataset/sensor_data.csv")

X = data[["Temperature", "Humidity"]]
encoder = LabelEncoder()
y = encoder.fit_transform(data["Status"])

# 2. Split data
X_train, X_test, y_train, y_test = train_test_split(
    X, y,
    test_size=0.25,
    random_state=42,
    stratify=y
)

# 3. Scale data
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# 4. Train models
models = {}

models["KNN"] = train_knn(
    X_train, X_test, y_train, y_test
)

models["Decision Tree"] = train_decision_tree(
    X_train, X_test, y_train, y_test
)

models["SVM"] = train_svm(
    X_train, X_test, y_train, y_test
)

# 5. Compare accuracy
print("\n===== MODEL COMPARISON =====")

for name, model in models.items():
    predictions = model.predict(X_test)
    accuracy = accuracy_score(y_test, predictions)

    print(f"{name}: {accuracy:.4f}")

from pathlib import Path
import joblib

# Xác định thư mục gốc của project
BASE_DIR = Path(__file__).resolve().parent.parent

# Tạo thư mục results bên trong project
RESULT_DIR = BASE_DIR / "results"
RESULT_DIR.mkdir(parents=True, exist_ok=True)

# Lưu các mô hình
for name, model in models.items():
    filename = name.lower().replace(" ", "_") + ".pkl"
    save_path = RESULT_DIR / filename

    joblib.dump(encoder, RESULT_DIR / "label_encoder.pkl")
    joblib.dump(model, save_path)
    print(f"Saved: {save_path}")

# Lưu bộ chuẩn hóa dữ liệu
joblib.dump(scaler, RESULT_DIR / "scaler.pkl")
joblib.dump(encoder, RESULT_DIR / "label_encoder.pkl")
print("\nModels saved successfully!")