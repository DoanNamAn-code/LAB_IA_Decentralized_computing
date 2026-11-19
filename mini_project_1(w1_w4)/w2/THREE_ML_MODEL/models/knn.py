from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score, classification_report


def train_knn(X_train, X_test, y_train, y_test):

    # Khoi tao mo hinh
    model = KNeighborsClassifier(n_neighbors=3)

    # Huan luyen
    model.fit(X_train, y_train)

    # Du doan
    predictions = model.predict(X_test)

    # Danh gia
    accuracy = accuracy_score(y_test, predictions)

    print("\n===== K-NN =====")
    print("Accuracy:", accuracy)
    print(classification_report(
        y_test,
        predictions,
        zero_division=0
    ))

    return model
