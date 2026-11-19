from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score, classification_report


def train_decision_tree(X_train, X_test, y_train, y_test):

    model = DecisionTreeClassifier(
        max_depth=3,
        random_state=42
    )

    model.fit(X_train, y_train)

    predictions = model.predict(X_test)

    accuracy = accuracy_score(y_test, predictions)

    print("\n===== DECISION TREE =====")
    print("Accuracy:", accuracy)
    print(classification_report(
        y_test,
        predictions,
        zero_division=0
    ))

    return model
