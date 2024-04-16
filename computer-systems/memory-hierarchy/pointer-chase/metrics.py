import csv
import datetime
import math
import time


class Users:
    def __init__(
        self,
        ids: list[int],
        names: list[str],
        ages: list[int],
    ) -> None:
        self.ids = ids
        self.names = names
        self.ages = ages


class Payments:
    def __init__(
        self,
        user_ids: list[int],
        amounts: list[float],
        times: list[datetime.datetime],
    ) -> None:
        self.user_ids = user_ids
        self.amounts = amounts
        self.times = times


def average_age(ages: list[int]):
    return sum(ages) / len(ages)


def average_payment_amount(amounts: list[float]):
    return sum(amounts) / len(amounts)


def stddev_payment_amount(amounts: list[float]):
    mean = average_payment_amount(amounts)
    n = len(amounts)
    squared_diffs = sum([(x - mean) ** 2 for x in amounts])
    return math.sqrt(squared_diffs / n)


def load_data():
    users = {}
    with open("users.csv") as f:
        test = [list(col) for col in zip(*csv.reader(f))]
        ids, names, ages, *_ = test
        users = Users([int(id) for id in ids], names, [int(age) for age in ages])
    with open("payments.csv") as f:
        amounts, ts, uids = [list(col) for col in zip(*csv.reader(f))]
        payments = Payments(
            [int(id) for id in uids],
            [int(amount) * 1e-2 for amount in amounts],
            [datetime.datetime.fromisoformat(t) for t in ts],
        )
    return users, payments


if __name__ == "__main__":
    t = time.perf_counter()
    users, payments = load_data()
    print(f"Data loading: {time.perf_counter() - t:.3f}s")
    t = time.perf_counter()
    assert abs(average_age(users.ages) - 59.626) < 0.01
    assert abs(stddev_payment_amount(payments.amounts) - 288684.849) < 0.01
    assert abs(average_payment_amount(payments.amounts) - 499850.559) < 0.01
    print(f"Computation {time.perf_counter() - t:.3f}s")
