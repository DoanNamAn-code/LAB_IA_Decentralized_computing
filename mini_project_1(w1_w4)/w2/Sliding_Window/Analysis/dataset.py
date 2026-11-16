
import pandas as pd
from pathlib import Path

from sliding_window import create_sliding_windows
from feature_extraction import extract_features
from labeling import assign_label


def generate_dataset(input_csv, output_csv):

    print("\n================================")
    print("       DATASET GENERATION")
    print("================================")

    df = pd.read_csv(input_csv)

    df = df.dropna(subset=["temperature", "humidity"])

    WINDOW_SIZE = 5
    STEP_SIZE = 1

    windows = create_sliding_windows(
        df,
        WINDOW_SIZE,
        STEP_SIZE
    )

    dataset = []

    for index, window in enumerate(windows):

        features = extract_features(window)

        label = assign_label(features)

        features["window_id"] = index
        features["start_time_ms"] = window["timestamp_ms"].iloc[0]
        features["end_time_ms"] = window["timestamp_ms"].iloc[-1]
        features["label"] = label

        dataset.append(features)

    result = pd.DataFrame(dataset)

    Path(output_csv).parent.mkdir(
        parents=True,
        exist_ok=True
    )

    result.to_csv(output_csv, index=False)

    print("Number of windows:", len(result))
    print("Dataset saved to:", output_csv)

    print("\n[DATASET PREVIEW]")
    print(result.to_string(index=False))

    return result
