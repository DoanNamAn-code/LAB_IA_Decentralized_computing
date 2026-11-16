def extract_features(window):

    features = {
        "temperature_mean": window["temperature"].mean(),
        "temperature_min": window["temperature"].min(),
        "temperature_max": window["temperature"].max(),
        "temperature_std": window["temperature"].std(),

        "humidity_mean": window["humidity"].mean(),
        "humidity_min": window["humidity"].min(),
        "humidity_max": window["humidity"].max(),
        "humidity_std": window["humidity"].std()
    }

    return features
