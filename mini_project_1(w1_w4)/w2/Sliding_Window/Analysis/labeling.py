def assign_label(features):

    if features["temperature_mean"] >= 30:
        return 1

    return 0
