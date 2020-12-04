use clap::{App, SubCommand, Arg};
use std::fs::File;
use std::io::{BufRead, BufReader};



fn read_input(file_path: &str) -> Vec<u16>
{
    let mut out: Vec<u16> = vec![];
    let file: File = File::open(file_path).unwrap();
    let buffer: BufReader<File> = BufReader::new(file);
    out.reserve(200);

    for line in buffer.lines()
    {
        out.push(line.unwrap().parse::<u16>().unwrap());
    }

    return out;
}

pub fn bin_search(data: &Vec<u16>, val: u16) -> (bool, u16)
{
    let mut start = 0;
    let mut end = data.len() as i32 - 1;
    let mut idx: i32;
    while start <= end
    {

        idx = (end + start) / 2;

        if data[idx as usize] > val
        {
            end = idx - 1;
        }
        else if data[idx as usize] < val
        {
            start = idx + 1;
        }
        else
        {
            return (true, data[idx as usize]);
        }
    }

    return (false, 0);
}

fn process(input_path: &str, part: u8)
{
    let mut data: Vec<u16> = read_input(input_path);
    
    let mut bth: Vec<u16> = vec![];
    let mut sth: Vec<u16> = vec![];
    bth.reserve(data.len());
    sth.reserve(data.len());

    for i in data.iter()
    {
        if *i < 1010u16
        {
            sth.push(*i);
        }
        if *i >= 1010u16
        {
            bth.push(*i);
        }
    }


    sth.sort();
    bth.sort();
    
    data.clear();
    data.extend(&sth);
    data.extend(&bth);

    if bth.len() > sth.len() || part == 2
    {
        data.reverse();
    }

    if part == 1
    {
        let mut vref: &Vec<u16> = &data;
        
        for i in data.iter()
        {
            if *i < 1010
            {
                vref = &bth;
            }
            else if *i >= 1010
            {
                vref = &sth;
            }

            let (res, value) = bin_search(&vref, 2020 - *i);
            if res
            {
                println!("resultado: {}", value as u32 * (*i) as u32);
                return;
            }
        }
    }
    else if part == 2
    {
        for i in data.iter()
        {
            if *i < 1010
            {
                for j in bth.iter()
                {
                    if*j + *i >= 2020 // || *j + *i > bth[0]
                    {
                        continue;
                    }

                    let (res, value) = bin_search(&bth, 2020 - (*i + *j) as u16);
                    if res
                    {
                        println!("1: {}, 2: {}, 3: {}", value, *i, *j);
                        println!("resultado: {}", value as u32 * (*i) as u32 * *j as u32);
                        return;
                    }
                }
            }
            else if *i >= 1010
            {
                for j in sth.iter()
                {
                    if*j + *i >= 2020 // || *j + *i > sth[0]
                    {
                        continue;
                    }

                    let (res, value) = bin_search(&sth, 2020 - (*i + *j));
                    if res
                    {
                        println!("1: {}, 2: {}, 3: {}", value, *i, *j);
                        println!("resultado: {}", (value as u32) * (*i as u32) * (*j as u32));
                        return;
                    }
                }
            }
        }
    }

}


fn main() {

    let arg_matches = App::new("adventofcode df1")
        .author("gabrielcfvg <gabrielcfvg@gmail.com")
        .arg(Arg::with_name("path")
            .required(false)
            .default_value("input.txt")
        )
        .subcommand(
            SubCommand::with_name("double")
        )
        .subcommand(
            SubCommand::with_name("triple")
        )
        .get_matches();

    
    let path: &str = arg_matches.value_of("path").unwrap();

    if let Some(_) = arg_matches.subcommand_matches("double")
    {
        process(path, 1);
    }
    else if let Some(_) = arg_matches.subcommand_matches("triple")
    {
        process(path, 2);
    }
    else
    {
        println!("subcommand required");
    }

}


#[test]
fn test_bin_search()
{
    let teste: Vec<u16> = vec![1, 2, 3, 4, 5, 6, 7, 8, 9];
    
    let (su, va) = bin_search(&teste, 1);
    assert!(su);
    println!("res: {}, value: {}", su, va);

    let (su, va) = bin_search(&teste, 9);
    assert!(su);
    println!("res: {}, value: {}", su, va);
    
    
    let (su, va) = bin_search(&teste, 10);
    assert!(!su);
    println!("res: {}, value: {}", su, va);
}