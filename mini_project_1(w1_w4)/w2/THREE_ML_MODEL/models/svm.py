from sklearn.svm import SVC
from sklearn.metrics import accuracy_score, classification_report


def train_svm(X_train, X_test, y_train, y_test):

    model = SVC(
        kernel="rbf",
        C=1.0,
        gamma="scale"
    )

    model.fit(X_train, y_train)

    predictions = model.predict(X_test)

    accuracy = accuracy_score(y_test, predictions)

    print("\n===== SVM =====")
    print("Accuracy:", accuracy)
    print(classification_report(
        y_test,
        predictions,
        zero_division=0
    ))

    return model
