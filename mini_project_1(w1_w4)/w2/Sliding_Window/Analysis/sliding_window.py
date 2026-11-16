def create_sliding_windows(df, window_size, step_size):

    windows = []

    for start in range(0, len(df) - window_size + 1, step_size):

        end = start + window_size

        window = df.iloc[start:end]

        windows.append(window)

    return windows
