class PersonNode:
    def __init__(self, name_value: str, age_value: int)->None:
        print("PersonNode __init__方法被调用了，添加了两个属性")
        self.name = name_value
        self.age = age_value

    def eat(self, food:str)->None:
        """
        吃饭--方法
        """
        print("{},{}岁，爱吃{}".format(self.name, self.age, food))

def main():
    node = PersonNode("陈超", 18)
    node.eat("beef, fish, meat")

if __name__ == "__main__":
    main()